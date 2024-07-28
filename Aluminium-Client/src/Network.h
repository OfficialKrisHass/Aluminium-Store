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

    void SendMessage(const char* msg);
    void RecieveMessage(NetworkMessage *out);
    uint32 RecieveMessages(NetworkMessage **out);
    
    void SetConnectionStatusChangedCallback(ConnectionStatusChangeCallback callback);

}
