#include <Network.h>

#include <steam/steamnetworkingsockets.h>
#include <steam/isteamnetworkingutils.h>

namespace Aluminium::Network {

    ConnectionState StateConvert(int32 steamState) {

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
