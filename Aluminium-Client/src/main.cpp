#include "Window.h"
#include "UI.h"
#include "Network.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <misc/cpp/imgui_stdlib.h>

#include <thread>

#define REGISTER_CHECK(condition, msg) if (!(condition)) { errMsg = msg; LogError(errMsg); return; }

namespace Aluminium {

    bool running = true;
    bool loggedIn = false;

    void Shutdown();

    void ConnectingUI();
    void LoginUI();
    void RegisterUI();
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
            else if (!loggedIn)
                LoginUI();
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

    std::string errMsg;

    std::string login;
    std::string password;
    bool logginIn = false;
    bool loginRequested = false;
    void LoginUI() {

        if (!logginIn) {

            RegisterUI();
            return;

        }

    }
    std::string username;
    std::string repeatPassword;
    void RegisterUI() {

        ImGui::PushItemFlag(ImGuiItemFlags_Disabled, loginRequested);

        ImGui::InputText("Email: ", &login);
        ImGui::InputText("Username: ", &username);
        ImGui::InputText("Password: ", &password);
        ImGui::InputText("Repeat Password: ", &repeatPassword);

        if (ImGui::Button("Register")) {

            REGISTER_CHECK(username[0] != ',', "Username can't begin with a comma");
            REGISTER_CHECK(login.find('@') != std::string::npos, "Invalid email");
            REGISTER_CHECK(password == repeatPassword, "Passwords don't match");

            std::string msg = "/Register/";
            msg += login + "," + username + "," + password;
            Network::SendMessage(msg.c_str());
            
            loginRequested = true;

        }

        if (!loginRequested) return;
        ImGui::PopItemFlag();

        Network::NetworkMessage response;
        Network::RecieveMessage(&response);
        if (!response) return;
        if (strcmp(response.msg, "/Register/Success") != 0) return;

        loginRequested = false;
        loggedIn = true;

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
