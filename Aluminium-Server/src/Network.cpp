#include "Network.h"

#include <steam/steamnetworkingsockets.h>
#include <steam/isteamnetworkingutils.h>

#include <iostream>

namespace Aluminium::Network {

    static void DebugOutput(ESteamNetworkingSocketsDebugOutputType type, const char* msg);
    static void OnConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* info);

    static ConnectionState StateConvert(ESteamNetworkingConnectionState steamState);

    ISteamNetworkingSockets* interface;
    HSteamListenSocket listen;

    ConnectionStatusChangeCallback connectionStatusChangeCallback = nullptr;

    void Initialize() {

        // Initialize GameNetworkingSockets

        std::cout << "Initializing GameNetworkingSockets\n";

        SteamDatagramErrMsg errMsg;
        if (!GameNetworkingSockets_Init(nullptr, errMsg))
            std::cout << "Failed to initialize GameNetworkingSockets!\n";

        SteamNetworkingUtils()->SetDebugOutputFunction(k_ESteamNetworkingSocketsDebugOutputType_Msg, DebugOutput);

        // Open the socket
        
        std::cout << "Opening server socket\n";

        SteamNetworkingIPAddr addr;
        addr.Clear();
        addr.m_port = ALUMINIUM_PORT;
        
        interface = SteamNetworkingSockets();

        SteamNetworkingConfigValue_t opt;
        opt.SetPtr(k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged, (void*) OnConnectionStatusChanged);
        listen = interface->CreateListenSocketIP(addr, 1, &opt);

        if (listen == k_HSteamListenSocket_Invalid) {

            std::cout << "Failed to open Server socket!\n";
            exit(-1);

        }

        std::cout << "Successfully opened server socket\n";


    }
    void Shutdown() {

        std::cout << "Closing server socket\n";
        interface->CloseListenSocket(listen);

        GameNetworkingSockets_Kill();

    }

    void SetConnectionStatucChangedCallback(ConnectionStatusChangeCallback callback) { connectionStatusChangeCallback = callback; }

    void DebugOutput(ESteamNetworkingSocketsDebugOutputType type, const char* msg) {

        std::cout << msg << "\n";

    }
    void OnConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* info) {

        connectionStatusChangeCallback(StateConvert(info->m_info.m_eState));

    }

    ConnectionState StateConvert(ESteamNetworkingConnectionState steamState) {

        switch (steamState) {

            case k_ESteamNetworkingConnectionState_None: return ConnectionState::None;
            case k_ESteamNetworkingConnectionState_ClosedByPeer: return ConnectionState::ClosedByPeer;
            case k_ESteamNetworkingConnectionState_ProblemDetectedLocally: return ConnectionState::LocalProblem;
            case k_ESteamNetworkingConnectionState_Connecting: return ConnectionState::Connecting;
            case k_ESteamNetworkingConnectionState_Connected: return ConnectionState::Connected;
            default: return ConnectionState::None;

        }

        return ConnectionState::None;

    }

}
