#pragma once

#include <Core.h>
#include <Network.h>

namespace Aluminium::Network {

    void Initialize();
    void Update();
    void Shutdown();

    void NetworkThread();

    bool ConnectToServer();
    bool IsConnected();

    void CloseConnection(Connection conn);

    void SendMessage(const std::string& msg);
    void RecieveMessage(Message *out);
    uint32 RecieveMessages(Message **out);
    
    void SetConnectionStatusChangedCallback(ConnectionStatusChangeCallback callback);

    typedef void (*MessageHandler)(const Message& message);
    void SetMessageHandler(MessageHandler handler);

}
