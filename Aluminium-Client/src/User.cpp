#include "User.h"
#include "UI.h"
#include "Window.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <misc/cpp/imgui_stdlib.h>

namespace Aluminium::User {

    std::string username;

    void LogIn() {

        Log("Logging In");

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

        Log("Logged in as {} with password {}", username, tmp);

    }

    std::string Name() { return username; }

}
