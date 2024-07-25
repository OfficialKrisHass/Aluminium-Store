#include "Network.h"
#include "User.h"

#include <thread>
#include <unordered_map>

namespace Aluminium {

    bool running = true;
    std::unordered_map<uint32, User> connectedUsers;

    void ConnectionStatusChangedCallback(Network::StatusChangeData data);

    int StartServer() {

        Log("Starting Aluminium Server");

        Network::Initialize();
        Network::SetConnectionStatusChangedCallback(ConnectionStatusChangedCallback);

        int i = 0;
        while (running) {

            Network::Update();

            std::this_thread::sleep_for(std::chrono::seconds(1));

            if (i == 10)
                running = false;

            i++;

        }

        Log("Shutting down Aluminium server");

        Network::Shutdown();

        Log("Aluminium server shut down successfully");

        return 0;

    }

    void ConnectionStatusChangedCallback(Network::StatusChangeData data) {

        using namespace Network;

        switch (data.state) {

            case ConnectionState::None: break;
            case ConnectionState::ClosedByPeer:
            case ConnectionState::LocalProblem: {

                if (data.oldState != ConnectionState::Connected) {

                    Network::CloseConnection(data.conn);
                    break;

                }

                if (data.state == ConnectionState::LocalProblem)
                    LogWarn("Encountered a local problem with user's {} connection", connectedUsers[data.conn].GetName());

                Log("User {} has disconnected from the server", connectedUsers[data.conn].GetName());

                connectedUsers.erase(data.conn);
                Network::CloseConnection(data.conn);

                break;

            }
            case ConnectionState::Connecting: {

                Log("Incoming connection from {}", data.connDescription);
                
                if (!Network::AcceptConnection(data.conn)) {

                    LogError("Could not accept connection (maybe already closed ?)");

                    Network::CloseConnection(data.conn);
                    break;

                }

                connectedUsers[data.conn] = User(data.conn, "User" + std::to_string(connectedUsers.size()));

            }
            case ConnectionState::Connected: break;
            default: break;

        }

    }

}

int main() {

    return Aluminium::StartServer();

}
