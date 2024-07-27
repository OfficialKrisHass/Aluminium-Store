#pragma once

#include <Core.h>
#include <Network.h>

namespace Aluminium::Network {

    void Initialize();
    void Update();
    void Shutdown();

    bool AcceptConnection(Connection conn);
    void CloseConnection(Connection conn);

    void SetConnectionStatusChangedCallback(ConnectionStatusChangeCallback callback);

}
