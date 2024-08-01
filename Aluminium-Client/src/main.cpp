#include "Window.h"
#include "UI.h"
#include "Network.h"

#include "SignInScreen.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <misc/cpp/imgui_stdlib.h>

#include <thread>

namespace Aluminium {

    bool running = true;
    bool signedIn = false;
    uint32 userID = 0;

    void Shutdown();

    void ConnectingUI();
    void StoreUI();

    void HandleMessage(const Network::Message& message);

    void OnConnectionStatusChanged(const Network::StatusChangeData& data);
    void OnWindowClose();

    int32 StartClient() {

        Log("Starting Aluminium Client");

        Window::Initialize(OnWindowClose);
        UI::Initialize();

        // Start the network and the network thread
        
        Network::Initialize();
        Network::SetConnectionStatusChangedCallback(OnConnectionStatusChanged);
        Network::SetMessageHandler(HandleMessage);

        std::thread networkThread(Network::NetworkThread);

        while (running) {

            Window::Update();
            UI::BeginFrame();

            Network::Update();

            if (!Network::IsConnected())
                ConnectingUI();
            else if (!signedIn)
                SignInScreen::UI();
            else
                StoreUI();

            UI::EndFrame();

        }

        networkThread.join();

        Shutdown();

        return 0;

    }
    void Shutdown() {

        Log("Shutting down");

        Network::Shutdown();

        UI::Shutdown();
        Window::Shutdown();

    }

    void ConnectingUI() {

        ImGui::Text("Connecting...");

    }
    void StoreUI() {

        ImGui::Text("Welcome to the Aluminium Store!");
        ImGui::Text("Your UserID is: %d", userID);

    }

    void HandleMessage(const Network::Message& message) {

        switch (message.type) {

            case MESSAGE_SIGNIN_SUCCESS: {

                userID = SignInScreen::SignInSuccess(message);
                signedIn = true;

                break;

            }
            case MESSAGE_SIGNIN_FAIL: SignInScreen::SignInFail(message); break;
            case MESSAGE_USER_SALT: SignInScreen::GotUserSalt(message); break;
            default: LogError("Invalid message type {} for message {}", message.type, message.msg); break;

        }

    }

    void OnConnectionStatusChanged(const Network::StatusChangeData& data) {

        using namespace Network;

        switch (data.state) {

            case ConnectionState::ClosedByPeer:
            case ConnectionState::LocalProblem: {
                
                running = false;

                if (data.oldState == ConnectionState::Connecting)
                    LogError("Timed out");
                if (data.state == ConnectionState::LocalProblem)
                    LogError("A local problem has been encountered, quitting now");
                else
                    Log("Disconnected from the server manually");

                Network::CloseConnection(data.conn);

                break;

            }
            default: break;

        }

    }
    void OnWindowClose() {

        Log("Window close has been requested");
        running = false;

    }

    bool IsRunning() { return running; }

}

int main() {

    return Aluminium::StartClient();

}
