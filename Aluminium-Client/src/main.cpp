#include "Window.h"
#include "UI.h"
#include "Network.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <misc/cpp/imgui_stdlib.h>

#include <thread>

namespace Aluminium {

    bool running = true;

    void Shutdown();

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

        bool test = false;
        Network::NetworkMessage msg;

        while (running) {

            Window::Update();
            UI::BeginFrame();

            if (!Network::IsConnected())
                ImGui::Text("Connecting...");
            else {

                ImGui::Text("Welcome to the aluminium Store!");

                if (!test) {

                    Network::SendMessage("Wadup m8"); 
                    test = true;

                }

                if (!msg)
                    Network::RecieveMessage(&msg);
                else {

                    ImGui::Text("Server Says: ");
                    ImGui::SameLine();
                    ImGui::Text(msg.msg);

                }

            }

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
