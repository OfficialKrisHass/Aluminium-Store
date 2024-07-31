#pragma once

#include "Core.h"

#define MESSAGE_NONE 0

#define MESSAGE_SIGNUP 1
#define MESSAGE_SIGNIN 2
#define MESSAGE_SIGNIN_SUCCESS 3
#define MESSAGE_SIGNIN_FAIL 4

#define MESSAGE_USER_SALT 5

#define MESSAGE(type) std::to_string(type) + ':'

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

    struct Message {

        uint8 type = MESSAGE_NONE;
        Connection conn = 0;

        char* msg = nullptr;
        uint32 size = 0;

        void SetType() {

            uint32 i = 0;
            for (; i < size; i++) {

                if (msg[i] == ':') break;
                type = type * 10 + (msg[i] - '0');

            }

            i++;
            msg += i;
            size -= i;

        }

        inline operator bool() const { return size != 0; }

    };

    typedef void (*ConnectionStatusChangeCallback)(const StatusChangeData& data);

    ConnectionState StateConvert(int32 steamState);

}
