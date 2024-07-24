#include "User.h"
#include "UI.h"
#include "Window.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <misc/cpp/imgui_stdlib.h>

#include <iostream>

namespace Aluminium::User {

    std::string username;

    void LogIn() {

        std::cout << "Logging in\n";

        std::string tmp;

        bool loggedIn = false;
        while (!loggedIn && Window::IsWindowOpen()) {

            Window::Update();
            UI::BeginFrame();

            ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;
            ImGuiWindowClass winClass;
            winClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
            ImGui::SetNextWindowClass(&winClass);

            ImGui::Begin("Log in page", nullptr, flags);

            ImGui::InputText("Username:", &username);
            ImGui::InputText("Password:", &tmp, ImGuiInputTextFlags_Password);

            if (ImGui::Button("Log in"))
                loggedIn = true;

            ImGui::End();

            UI::EndFrame();

        }

        std::cout << "Logged in as " << username << " with password " << tmp << "\n";

    }

    std::string Name() { return username; }

}
