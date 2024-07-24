#include "Window.h"
#include "UI.h"
#include "Network.h"
#include "User.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <misc/cpp/imgui_stdlib.h>

#include <iostream>

namespace Aluminium {

    void Shutdown();

    int32 StartClient() {

        std::cout << "Starting Aluminium Client\n";

        Window::Initialize();
        UI::Initialize();

        Network::Initialize();

        if (!Network::ConnectToServer()) {

            std::cout << "Failed to connect to the Aluminium Server!\n";

            Shutdown();
            return -1;

        }
        std::cout << "Succesfully connected to an Aluminium Server\n";

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

        std::cout << "Shutting down\n";

        Network::Shutdown();

        UI::Shutdown();
        Window::Shutdown();

    }

}

int main() {

    std::cout << "Hello, World!\n";

    return Aluminium::StartClient();

}
