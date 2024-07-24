#include "Network.h"

#include <steam/steamnetworkingsockets.h>
#include <steam/isteamnetworkingutils.h>

#include <iostream>

namespace Aluminium::Network {

    HSteamNetConnection connection;
    ISteamNetworkingSockets* interface;

    void OnConnectionStatucChanged(SteamNetConnectionStatusChangedCallback_t* info);
    void DebugOutput(ESteamNetworkingSocketsDebugOutputType type, const char* msg);

    void Initialize() {

        std::cout << "Initializing GameNetworkingSockets\n";

        SteamDatagramErrMsg errMsg;
        if (!GameNetworkingSockets_Init(nullptr, errMsg))
            std::cout << "Failed to initialize GameNetworkingSockets\n";

        SteamNetworkingUtils()->SetDebugOutputFunction(k_ESteamNetworkingSocketsDebugOutputType_Msg, DebugOutput);

        interface = SteamNetworkingSockets();

        std::cout << "Successfully initialized GameNetworkingSockets\n";

    }
    void Shutdown() {

        std::cout << "Shutting down GameNetworkingSockets\n";
        GameNetworkingSockets_Kill();

    }
    
    bool ConnectToServer() {

        // Get server address

        SteamNetworkingIPAddr serverAddr;
        serverAddr.Clear();
        serverAddr.ParseString("::1");
        serverAddr.m_port = ALUMINIUM_PORT;
        if (serverAddr.IsIPv6AllZeros()) {

            std::cout << "Could not parse IPv6 address\n";
            return false;

        }

        char addrBuf[SteamNetworkingIPAddr::k_cchMaxString];
        serverAddr.ToString(addrBuf, SteamNetworkingIPAddr::k_cchMaxString, true);
        std::cout << "Connecting to Aluminium server at address " << addrBuf << "\n";

        // Connect to the server

        SteamNetworkingConfigValue_t opt;
        opt.SetPtr(k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged, (void*) OnConnectionStatucChanged);
        connection = interface->ConnectByIPAddress(serverAddr, 1, &opt);

        // Error check

        if (connection != k_HSteamNetConnection_Invalid) return true;

        std::cout << "Could not connect to server at adress " << addrBuf << "\n";
        return false;

    }
    void DebugOutput(ESteamNetworkingSocketsDebugOutputType type, const char* msg) {

        std::cout << msg << "\n";

    }
    void OnConnectionStatucChanged(SteamNetConnectionStatusChangedCallback_t* info) {

        //

    }

}
