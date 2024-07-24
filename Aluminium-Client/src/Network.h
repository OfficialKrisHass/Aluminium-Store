#pragma once

#include "Core.h"

#define ALUMINIUM_PORT 3001

namespace Aluminium::Network {

    void Initialize();
    void Shutdown();

    bool ConnectToServer();

}
