#include "Network.h"

#include <steam/steamnetworkingsockets.h>
#include <steam/isteamnetworkingutils.h>

namespace Aluminium::Network {

    static void DebugOutput(ESteamNetworkingSocketsDebugOutputType type, const char* msg);
    static void OnConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* info);

    ISteamNetworkingSockets* interface;
    HSteamListenSocket listen;
    HSteamNetPollGroup pollGroup;

    ConnectionStatusChangeCallback connectionStatusChangeCallback = nullptr;

    void Initialize() {

        // Initialize GameNetworkingSockets

        Log("Initializing GameNetworkingSockets");

        SteamDatagramErrMsg errMsg;
        if (!GameNetworkingSockets_Init(nullptr, errMsg))
            LogError("Failed to initialize GameNetworkingSockets");

        SteamNetworkingUtils()->SetDebugOutputFunction(k_ESteamNetworkingSocketsDebugOutputType_Msg, DebugOutput);

        // Open the socket
        
        Log("Opening server socket");

        SteamNetworkingIPAddr addr;
        addr.Clear();
        addr.m_port = ALUMINIUM_PORT;
        
        interface = SteamNetworkingSockets();

        SteamNetworkingConfigValue_t opt;
        opt.SetPtr(k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged, (void*) OnConnectionStatusChanged);
        listen = interface->CreateListenSocketIP(addr, 1, &opt);

        if (listen == k_HSteamListenSocket_Invalid) {

            LogError("Failed to open Server socket");
            exit(-1);

        }

        // Create poll group

        pollGroup = interface->CreatePollGroup();
        if (pollGroup == k_HSteamNetPollGroup_Invalid) {

            LogError("Failed to open Server socket");
            exit(-1);

        }

        Log("Successfully opened server socket");


    }
    void Update() {

        interface->RunCallbacks();

    }
    void Shutdown() {

        Log("Closing server socket");
        interface->CloseListenSocket(listen);

        GameNetworkingSockets_Kill();

    }

    bool AcceptConnection(Connection conn) {

        if (interface->AcceptConnection(conn) != k_EResultOK) return false;
        if (!interface->SetConnectionPollGroup(conn, pollGroup)) {

            LogError("Could not set Connection #{} to poll group", conn);
            interface->CloseConnection(conn, 0, nullptr, false);

            return false;


        }

        return true;

    }
    void CloseConnection(Connection conn) {

        interface->CloseConnection(conn, 0, nullptr, false);

    }

    void SendMessage(Connection conn, const char* msg) {

        AL_ASSERT(conn != k_HSteamNetConnection_Invalid, "Can not send message to an invalid connection. Message: {}", msg);

        EResult res = interface->SendMessageToConnection(conn, msg, (uint32) strlen(msg), k_nSteamNetworkingSend_Reliable, nullptr); 
        AL_ASSERT(res == k_EResultOK, "Could not send message to server. Message: {}", msg);

    }
    void RecieveMessage(NetworkMessage* out) {

        ISteamNetworkingMessage* incomingMsg = nullptr;
        if (interface->ReceiveMessagesOnPollGroup(pollGroup, &incomingMsg, 1) == 0) return;

        out->conn = incomingMsg->m_conn;
        out->msg = (const char*) incomingMsg->m_pData;
        out->size = incomingMsg->m_cbSize;

    }
    uint32 RecieveMessages(NetworkMessage** out) {

        ISteamNetworkingMessage* incomingMsg = nullptr;
        uint32 num = interface->ReceiveMessagesOnPollGroup(pollGroup, &incomingMsg, 100);
        if (num == 0) return 0;

        *out = new NetworkMessage[num];
        for (uint32 i = 0; i < num; i++) {

            NetworkMessage* msg = out[i];

            msg->conn = incomingMsg->m_conn;
            msg->msg = (const char*) incomingMsg->m_pData;
            msg->size = incomingMsg->m_cbSize;   

            incomingMsg++;

        }

        return num;

    }

    void SetConnectionStatusChangedCallback(ConnectionStatusChangeCallback callback) { connectionStatusChangeCallback = callback; }

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

    }

}
