#include "Network.h"

#include <steam/steamnetworkingsockets.h>
#include <steam/isteamnetworkingutils.h>

namespace Aluminium::Network {

    HSteamNetConnection connection;
    ISteamNetworkingSockets* interface;

    void OnConnectionStatucChanged(SteamNetConnectionStatusChangedCallback_t* info);
    void DebugOutput(ESteamNetworkingSocketsDebugOutputType type, const char* msg);

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
        Log("Connecting to Aluminium server at address {}", addrBuf);

        // Connect to the server

        SteamNetworkingConfigValue_t opt;
        opt.SetPtr(k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged, (void*) OnConnectionStatucChanged);
        connection = interface->ConnectByIPAddress(serverAddr, 1, &opt);

        // Error check

        if (connection != k_HSteamNetConnection_Invalid) return true;

        LogError("Could not connect to server at address {}", addrBuf);
        return false;

    }
    void DebugOutput(ESteamNetworkingSocketsDebugOutputType type, const char* msg) {

        Log(msg);

    }
    void OnConnectionStatucChanged(SteamNetConnectionStatusChangedCallback_t* info) {

        //

    }

}
