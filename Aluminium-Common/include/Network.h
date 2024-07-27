#pragma once

#include "Core.h"

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

    typedef void (*ConnectionStatusChangeCallback)(const StatusChangeData& data);

    ConnectionState StateConvert(int32 steamState);

}
