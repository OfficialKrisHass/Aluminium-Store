#include "Window.h"
#include "UI.h"
#include "Network.h"
#include "User.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <misc/cpp/imgui_stdlib.h>

namespace Aluminium {

    void Shutdown();

    int32 StartClient() {

        Log("Starting Aluminium Client");

        Window::Initialize();
        UI::Initialize();

        Network::Initialize();

        if (!Network::ConnectToServer()) {

            LogError("Failed to connect to the Aluminium Server");

            Shutdown();
            return -1;

        }
        Log("Succesfully connected to an Aluminium Server");

        User::LogIn();

        while (Window::IsWindowOpen()) {

            Window::Update();
            UI::BeginFrame();

            ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;
            ImGuiWindowClass winClass;
            winClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
            ImGui::SetNextWindowClass(&winClass);

            ImGui::Begin("Main", nullptr, flags);

            ImGui::Text("Welcome to the aluminium store");

            ImGui::End();

            UI::EndFrame();

        }

        Shutdown();

        return 0;

    }

    void Shutdown() {

        Log("Shutting down");

        Network::Shutdown();

        UI::Shutdown();
        Window::Shutdown();

    }

}

int main() {

    return Aluminium::StartClient();

}
