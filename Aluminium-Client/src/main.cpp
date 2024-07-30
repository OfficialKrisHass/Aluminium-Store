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

    void Shutdown();

    void ConnectingUI();
    void StoreUI();

    void OnConnectionStatusChanged(const Network::StatusChangeData& data);
    void OnWindowClose();

    int32 StartClient() {

        Log("Starting Aluminium Client");

        Window::Initialize(OnWindowClose);
        UI::Initialize();

        // Start the network and the network thread
        
        Network::Initialize();
        Network::SetConnectionStatusChangedCallback(OnConnectionStatusChanged);
        std::thread networkThread(Network::Update);

        while (running) {

            Window::Update();
            UI::BeginFrame();

            if (!Network::IsConnected())
                ConnectingUI();
            else if (!signedIn)
                SignInScreen::UI(&signedIn);
            else
                StoreUI();

            UI::EndFrame();

        }

        networkThread.join();

        Shutdown();

        return 0;

    }


    void ConnectingUI() {

        ImGui::Text("Connecting...");

    }
    void StoreUI() {

        ImGui::Text("Welcome to the Aluminium Store!");

    }

    void Shutdown() {

        Log("Shutting down");

        Network::Shutdown();

        UI::Shutdown();
        Window::Shutdown();

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
