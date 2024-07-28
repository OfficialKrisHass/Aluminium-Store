#include "Network.h"
#include "main.h"

#include <steam/steamnetworkingsockets.h>
#include <steam/isteamnetworkingutils.h>

#include <thread>

namespace Aluminium::Network {

    bool connected = false;

    HSteamNetConnection connection;
    ISteamNetworkingSockets* interface;

    ConnectionStatusChangeCallback connectionStatusChangeCallback = nullptr;

    static void OnConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* info);
    static void DebugOutput(ESteamNetworkingSocketsDebugOutputType type, const char* msg);

    void Initialize() {

        Log("Initializing GameNetworkingSockets");

        SteamDatagramErrMsg errMsg;
        if (!GameNetworkingSockets_Init(nullptr, errMsg)) {

            LogError("Failed to initialize GameNetworkingSockets");
            exit(-1);

        }

        SteamNetworkingUtils()->SetDebugOutputFunction(k_ESteamNetworkingSocketsDebugOutputType_Msg, DebugOutput);

        interface = SteamNetworkingSockets();

        Log("Successfully initialized GameNetworkingSockets");

    }
    void Update() {

        if (!Network::ConnectToServer()) {

            LogError("Could not connect to the Aluminium server");
            exit(-1);

        }

        while (IsRunning()) {

            interface->RunCallbacks();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));

        }


    }
    void Shutdown() {

        Log("Shutting down GameNetworkingSockets");
        GameNetworkingSockets_Kill();

    }

    bool ConnectToServer() {

        // Get server address

        SteamNetworkingIPAddr serverAddr;
        serverAddr.Clear();
        serverAddr.ParseString("::1");
        serverAddr.m_port = ALUMINIUM_PORT;
        if (serverAddr.IsIPv6AllZeros()) {

            LogError("Could not parse IPv6 address");
            return false;

        }

        char addrBuf[SteamNetworkingIPAddr::k_cchMaxString];
        serverAddr.ToString(addrBuf, SteamNetworkingIPAddr::k_cchMaxString, true);
        Log("Connecting to an Aluminium server at address {}", addrBuf);

        // Connect to the server

        SteamNetworkingConfigValue_t opt;
        opt.SetPtr(k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged, (void*) OnConnectionStatusChanged);
        connection = interface->ConnectByIPAddress(serverAddr, 1, &opt);

        // Error check

        if (connection != k_HSteamNetConnection_Invalid) return true;

        LogError("Could not connect to the Aluminium server at address {}", addrBuf);
        return false;

    }
    bool IsConnected() { return connected; }

    void CloseConnection(Connection conn) {

        interface->CloseConnection(conn, 0, nullptr, false);
        connection = k_HSteamNetConnection_Invalid;

    }

    void SendMessage(const char* msg) {

        AL_ASSERT(connection != k_HSteamNetConnection_Invalid, "Can't send message when not connected. Message: {}", msg);

        EResult res = interface->SendMessageToConnection(connection, msg, (uint32) strlen(msg), k_nSteamNetworkingSend_Reliable, nullptr);
        AL_ASSERT(res == k_EResultOK, "Could not send message to server. Message: {}", msg);

    }
    void RecieveMessage(NetworkMessage *out) {

        ISteamNetworkingMessage* incomingMsg = nullptr;
        if (interface->ReceiveMessagesOnConnection(connection, &incomingMsg, 1) == 0) return;
        AL_ASSERT(incomingMsg->m_conn == connection, "Recieved message from a different connection {}, expected {}", incomingMsg->m_conn, connection);

        out->conn = incomingMsg->m_conn;
        out->msg = (const char*) incomingMsg->m_pData;
        out->size = incomingMsg->m_cbSize;

    }
    uint32 RecieveMessages(NetworkMessage **out) {

        ISteamNetworkingMessage* incomingMsg = nullptr;
        uint32 num = interface->ReceiveMessagesOnConnection(connection, &incomingMsg, 100);
        if (num == 0) return 0;

        *out = new NetworkMessage[num];
        for (uint32 i = 0; i < num; i++) {

            NetworkMessage* msg = out[i];
            AL_ASSERT(incomingMsg->m_conn == connection, "Recieved message from a different connection {}, expected {}", incomingMsg->m_conn, connection);

            msg->conn = incomingMsg->m_conn;
            msg->msg = (const char*) incomingMsg->m_pData;
            msg->size = incomingMsg->m_cbSize;   

            incomingMsg++;

        }

        return num;

    }

    void SetConnectionStatusChangedCallback(ConnectionStatusChangeCallback callback) {

        connectionStatusChangeCallback = callback;

    }

    void DebugOutput(ESteamNetworkingSocketsDebugOutputType type, const char* msg) {

        //Log(msg);

    }
    void OnConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* info) {

        StatusChangeData data;

        data.state = StateConvert(info->m_info.m_eState);
        data.oldState = StateConvert(info->m_eOldState);
        data.conn = info->m_hConn;
        data.connDescription = info->m_info.m_szConnectionDescription;

        connectionStatusChangeCallback(data);

        if (data.state != ConnectionState::Connected) return;
        
        connected = true;
        Log("Successfully connected to the Aluminium server"); 

    }

}
