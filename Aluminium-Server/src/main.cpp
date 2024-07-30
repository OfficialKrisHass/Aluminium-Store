#include "Network.h"
#include "Database.h"
#include "User.h"

#include <thread>
#include <unordered_map>

namespace Aluminium {

    bool running = true;
    std::unordered_map<uint32, User> connectedUsers;

    void HandleMessage(const Network::NetworkMessage& message);

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
            if (message)
                HandleMessage(message);

            std::this_thread::sleep_for(std::chrono::milliseconds(100));

        }

        Log("Shutting down Aluminium server");

        Network::Shutdown();

        Log("Aluminium server shut down successfully");

        return 0;

    }

    void HandleMessage(const Network::NetworkMessage& message) {

        uint64 begin = strlen("/SignIn/");
        const std::string msg = message.msg;

        if (msg.find("/SignUp/") != std::string::npos) {

            std::string email = msg.substr(begin, msg.find_first_of(' ', begin) - begin);
            begin += email.size() + 1;

            std::string username = msg.substr(begin, msg.find_first_of(' ', begin) - begin);
            begin += username.size() + 1;

            std::string password = msg.substr(begin);

            Log("Singing up user {}", email);

            Database::AddUser(email.c_str(), username.c_str(), password.c_str());
            Network::SendMessage(message.conn, "/SignIn/Success");

            return;

        }
        if (msg.find("/SignIn/") != std::string::npos) {

            std::string login = msg.substr(begin, msg.find_first_of(' ', begin) - begin);
            begin += login.size() + 1;

            std::string password = msg.substr(begin);

            Log("Signing in user {}", login);
            if (!Database::GetUserFromEmail(login.c_str()) && !Database::GetUserFromName(login.c_str())) {

                LogError("Failed to Sign In, invalid login");
                Network::SendMessage(message.conn, "/SignIn/Invalid login");

                return;

            }
            if (!Database::CheckUserPassword(login.c_str(), password.c_str())) {

                LogError("Failed to Sign In, invalid password");
                Network::SendMessage(message.conn, "/SignIn/Invalid password");

                return;

            }

            Log("Successfully logged in user {}", login);
            Network::SendMessage(message.conn, "/SignIn/Success");

            return;

        }

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
