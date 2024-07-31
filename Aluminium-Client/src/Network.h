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

    void SendMessage(const std::string& msg);
    void RecieveMessage(Message *out);
    uint32 RecieveMessages(Message **out);
    
    void SetConnectionStatusChangedCallback(ConnectionStatusChangeCallback callback);

}
