#pragma once

#define ALUMINIUM_PORT 3001

namespace Aluminium::Network {

    enum class ConnectionState {

        None = 0,
        ClosedByPeer = 1,
        LocalProblem = 2,
        Connecting = 3,
        Connected = 4,

    };

    void Initialize();
    void Shutdown();

    typedef void (*ConnectionStatusChangeCallback)(ConnectionState state);
    void SetConnectionStatucChangedCallback(ConnectionStatusChangeCallback callback);

}
