#pragma once

#include <Core.h>

namespace Aluminium::Network {

    enum class ConnectionState {

        None = 0,
        ClosedByPeer = 1,
        LocalProblem = 2,
        Connecting = 3,
        Connected = 4,

    };

    typedef uint32 Connection;
    struct StatusChangeData {

        ConnectionState state;
        ConnectionState oldState;
        
        Connection conn;
        const char* connDescription;

    };

    void Initialize();
    void Update();
    void Shutdown();

    bool AcceptConnection(Connection conn);
    void CloseConnection(Connection conn);

    typedef void (*ConnectionStatusChangeCallback)(StatusChangeData data);
    void SetConnectionStatusChangedCallback(ConnectionStatusChangeCallback callback);

}
