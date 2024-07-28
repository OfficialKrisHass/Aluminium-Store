#pragma once

#include <Core.h>
#include <Network.h>

namespace Aluminium::Network {

    void Initialize();
    void Update();
    void Shutdown();

    bool AcceptConnection(Connection conn);
    void CloseConnection(Connection conn);

    void SendMessage(Connection conn, const char* msg);
    void RecieveMessage(NetworkMessage* out);
    uint32 RecieveMessages(NetworkMessage** out);

    void SetConnectionStatusChangedCallback(ConnectionStatusChangeCallback callback);

}
