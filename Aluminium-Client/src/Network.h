#pragma once

#include <Core.h>
#include <Network.h>

namespace Aluminium::Network {

    void Initialize();
    void Update();
    void Shutdown();

    bool ConnectToServer();
    bool IsConnected();

    void CloseConnection(Connection conn);
    
    void SetConnectionStatusChangedCallback(ConnectionStatusChangeCallback callback);

}
