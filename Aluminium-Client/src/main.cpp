#include "Window.h"
#include "UI.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <misc/cpp/imgui_stdlib.h>

#include <iostream>

namespace Aluminium {

    bool loggedIn = false;

    std::string username;
    std::string password;

    void LogIn();

    void StartClient() {

        Window::Initialize();
        UI::Initialize();

        while (Window::IsWindowOpen()) {

            Window::Update();
            UI::BeginFrame();

            ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;
            ImGuiWindowClass winClass;
            winClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
            ImGui::SetNextWindowClass(&winClass);

            ImGui::Begin("Main", nullptr, flags);

            if (!loggedIn)
                LogIn();
            else
                ImGui::Text("Welcome to the Aluminium store!");

            ImGui::End();

            UI::EndFrame();

        }

        UI::Shutdown();
        Window::Shutdown();

    }

    void LogIn() {

        ImGui::Begin("Log in");

        ImGui::InputText("Username", &username);
        ImGui::InputText("Password", &password, ImGuiInputTextFlags_Password);

        if (ImGui::Button("Log in")) {

            std::cout << "Logged in as user " << username << " with password " << password << "\n";
            loggedIn = true;

        }

        ImGui::End();

    }

}

int main() {

    std::cout << "Hello, World!\n";

    Aluminium::StartClient();
    return 0;

}
