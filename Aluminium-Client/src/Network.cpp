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

    void SetConnectionStatusChangedCallback(ConnectionStatusChangeCallback callback) {

        connectionStatusChangeCallback = callback;

    }

    void DebugOutput(ESteamNetworkingSocketsDebugOutputType type, const char* msg) {

        Log(msg);

    }
    void OnConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* info) {

        StatusChangeData data;

        data.state = StateConvert(info->m_info.m_eState);
        data.oldState = StateConvert(info->m_eOldState);
        data.conn = info->m_hConn;
        data.connDescription = info->m_info.m_szConnectionDescription;

        connectionStatusChangeCallback(data);

        if (data.state != ConnectionState::Connected) return;
        
        Log("Successfully connected to the Aluminium server"); 
        connected = true;

    }

}
