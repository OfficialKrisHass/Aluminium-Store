#pragma once

#include <Core.h>

namespace Aluminium::Network { struct Message; }

namespace Aluminium::SignInScreen {

    void UI();

    uint32 SignInSuccess(const Network::Message& message);
    void SignInFail(const Network::Message& message);
    void GotUserSalt(const Network::Message& message);

}
