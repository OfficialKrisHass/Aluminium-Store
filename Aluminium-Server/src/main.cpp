#include "Network.h"
#include "Database.h"
#include "User.h"

#include <thread>
#include <unordered_map>

namespace Aluminium {

    bool running = true;
    std::unordered_map<Network::Connection, User> connectedUsers;

    void HandleMessage(const Network::Message& message);

    void SignUpUser(const Network::Message& message);
    void SignInUser(const Network::Message& message);
    void RetrieveUserSalt(const Network::Message& message);

    void ConnectionStatusChangedCallback(const Network::StatusChangeData& data);

    int StartServer(int argc, char** argv) {

        Log("Starting Aluminium Server");

        Network::Initialize();
        Network::SetConnectionStatusChangedCallback(ConnectionStatusChangedCallback);

        Database::Initialize(argv[1]);

        while (running) {

            Network::Update();

            Network::Message message;
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

    void HandleMessage(const Network::Message& message) {

        switch (message.type) {
            
            case MESSAGE_SIGNUP: SignUpUser(message); break; 
            case MESSAGE_SIGNIN: SignInUser(message); break;
            case MESSAGE_USER_SALT: RetrieveUserSalt(message); break;
            default: LogError("Invalid message type {}", (uint8) message.type);

        }

    }

    void SignUpUser(const Network::Message& message) {

        std::string vals[4];
        uint32 index = 0;
        for (uint32 i = 0; i < message.size; i++) {

            if (message.msg[i] == ' ') {

                index++;
                continue;

            }
            vals[index].push_back(message.msg[i]);

        }

        Log("Signing up user {} {}", vals[0], vals[1]);

        connectedUsers[message.conn] = Database::AddUser(vals[0], vals[1], vals[2], vals[3]);
        Network::SendMessage(message.conn, MESSAGE(MESSAGE_SIGNIN_SUCCESS) + std::to_string(connectedUsers.at(message.conn).GetID()));

        Log("Successfuly signed up user");

    }
    void SignInUser(const Network::Message& message) {

        std::string vals[2];
        uint32 index = 0;
        for (uint32 i = 0; i < message.size; i++) {

            if (message.msg[i] == ' ') {

                index++;
                continue;

            }
            vals[index].push_back(message.msg[i]);

        }

        Log("Signing in user {}", vals[0]);

        connectedUsers[message.conn] = Database::GetUser(vals[0]);
        User& user = connectedUsers.at(message.conn);

        if (!user || !Database::CheckUserPassword(user.GetID(), vals[1])) {

            Log("Failed to sign in user, invalid credentials");

            Network::SendMessage(message.conn, std::to_string(MESSAGE_SIGNIN_FAIL) + ":Invalid credentials");
            connectedUsers.erase(message.conn);

            return;

        }

        Network::SendMessage(message.conn, MESSAGE(MESSAGE_SIGNIN_SUCCESS) + std::to_string(user.GetID()));

        Log("Successfuly signed in user");

    }
    void RetrieveUserSalt(const Network::Message& message) {

        std::string login = message.msg;
        std::string salt;

        Log("Retrieving user salt for login {}", login);
        Database::GetUserSalt(login, salt);

        if (salt.empty()) {

            LogError("Could not get user salt");
            Network::SendMessage(message.conn, MESSAGE(MESSAGE_SIGNIN_FAIL) + "Could not get user salt");

            return;

        }

        Network::SendMessage(message.conn, MESSAGE(MESSAGE_USER_SALT) + salt);

        Log("Successfuly retrieved and sent user salt");

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

                connectedUsers[data.conn] = User();

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
