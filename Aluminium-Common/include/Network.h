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

        std::string msg = ""; 

        void SetMessage(char* data, uint32 len) {

            msg.reserve(len);

            uint32 i = 0;
            for (; i < len; i++) {

                if (data[i] == ':') break;
                type = type * 10 + (data[i] - '0');

            }
            for (++i; i < len; i++)
                msg.push_back(data[i]);

        }

        inline operator bool() const { return !msg.empty(); }

    };

    typedef void (*ConnectionStatusChangeCallback)(const StatusChangeData& data);

    ConnectionState StateConvert(int32 steamState);

}
