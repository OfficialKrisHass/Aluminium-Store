#include "Network.h"
#include "Database.h"
#include "User.h"

#include <thread>
#include <unordered_map>

namespace Aluminium {

    bool running = true;
    std::unordered_map<uint32, User> connectedUsers;

    void ConnectionStatusChangedCallback(const Network::StatusChangeData& data);

    int StartServer(int argc, char** argv) {

        Log("Starting Aluminium Server");

        Network::Initialize();
        Network::SetConnectionStatusChangedCallback(ConnectionStatusChangedCallback);

        Database::Initialize(argv[1]);

        while (running) {

            Network::Update();

            Network::NetworkMessage message;
            Network::RecieveMessage(&message);
            if (message) {

                std::string msg = message.msg;
                uint64 i = msg.find("/Register/");
                if (i != std::string::npos) {

                    i += 10;

                    std::string email = msg.substr(i, msg.find(',', i) - i);
                    i += email.size() + 1;

                    std::string username = msg.substr(i, msg.find(',', i) - i);
                    i += username.size() + 1;

                    std::string password = msg.substr(i, msg.find(',', i) - i);
                    i += password.size() + 1; 

                    Database::AddUser(email.c_str(), username.c_str(), password.c_str());
                    Network::SendMessage(message.conn, "/Register/Success");

                }

            }

            std::this_thread::sleep_for(std::chrono::milliseconds(100));

        }

        Log("Shutting down Aluminium server");

        Network::Shutdown();

        Log("Aluminium server shut down successfully");

        return 0;

    }

    void ConnectionStatusChangedCallback(const Network::StatusChangeData& data) {

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

int main(int argc, char** argv) {

    AL_ASSERT(argc > 1, "Missing database password in arguments");
    return Aluminium::StartServer(argc, argv);

}
