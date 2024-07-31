#pragma once

#include <Core.h>
#include <Network.h>

namespace Aluminium::Network {

    void Initialize();
    void Update();
    void Shutdown();

    bool AcceptConnection(Connection conn);
    void CloseConnection(Connection conn);

    void SendMessage(Connection conn, const std::string& msg);
    void RecieveMessage(Message* out);
    uint32 RecieveMessages(Message** out);

    void SetConnectionStatusChangedCallback(ConnectionStatusChangeCallback callback);

}
