#include "Network.h"

#include <steam/steamnetworkingsockets.h>
#include <steam/isteamnetworkingutils.h>

namespace Aluminium::Network {

    static void DebugOutput(ESteamNetworkingSocketsDebugOutputType type, const char* msg);
    static void OnConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* info);

    ISteamNetworkingSockets* interface;
    HSteamListenSocket listen;

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

        if (interface->AcceptConnection(conn) == k_EResultOK) return true;
        return false;

    }
    void CloseConnection(Connection conn) {

        interface->CloseConnection(conn, 0, nullptr, false);

    }

    void SetConnectionStatusChangedCallback(ConnectionStatusChangeCallback callback) { connectionStatusChangeCallback = callback; }

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

    }

}
