#include "SignInScreen.h"
#include "Network.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <misc/cpp/imgui_stdlib.h>

#define VERIFY(condition, msg) if (!(condition)) { errMsg = msg; LogError(msg); return; }

namespace Aluminium::SignInScreen {

    static std::string errMsg;

    bool signInRequested = false;
    std::string login;
    std::string password;

    bool signUp = true;
    std::string username;
    std::string repeatPassword;

    void SignIn();
    void SignUp();

    void UI(bool* signedIn) {

        ImGui::PushItemFlag(ImGuiItemFlags_Disabled, signInRequested);

        if (!signUp)
            SignIn();
        else
            SignUp();

        ImGui::SameLine();
        if (ImGui::Button(signUp ? "Already have an account ? Sign In" : "New ? Sign Up"))
            signUp = !signUp;

        if (!errMsg.empty()) {

            ImGui::PushStyleColor(ImGuiCol_Text, { 1.0f, 0.0f, 0.0f, 1.0f });
            ImGui::Text(errMsg.c_str());
            ImGui::PopStyleColor();

        }

        if (!signInRequested) return;
        ImGui::PopItemFlag();

        Network::NetworkMessage response;
        Network::RecieveMessage(&response);
        if (!response) return;

        signInRequested = false;
        login.clear();
        username.clear();
        password.clear();
        repeatPassword.clear();
        if (strcmp(response.msg, "/SignIn/Success") != 0) {

            errMsg = "Failed to ";
            errMsg += (signUp ? "Sign Up" : "Sign In");
            LogError("Failed to sign in or sign up");

            return;

        }

        *signedIn = true;
        Log("Successfully signed In!");


    }

    void SignIn() {

        ImGui::InputText("Username or email: ", &login);
        ImGui::InputText("Password: ", &password);

        if (!ImGui::Button("Sign In")) return;
        Log("Sending Sign in request");

        std::string msg = "/SignIn/";
        msg += login + ' ' + password;
        Log(msg.c_str());
        Network::SendMessage(msg.c_str());

        signInRequested = true;

    }
    void SignUp() {

        ImGui::InputText("Email: ", &login);
        ImGui::InputText("Username: ", &username);
        ImGui::InputText("Password: ", &password);
        ImGui::InputText("Repeat Password: ", &repeatPassword);

        if (!ImGui::Button("Sign Up")) return;
        Log("Attempting Sign up");

        VERIFY(username.find(' ') == std::string::npos, "Username can't contain a space");
        VERIFY(username.length() < 65, "Username can't be longer than 64 characters");

        VERIFY(login.find('@') != std::string::npos, "Invalid email");
        VERIFY(login.find(' ') == std::string::npos, "Email can't contain a space");
        VERIFY(login.length() < 256, "Email can't be longer than 255 characters");

        VERIFY(password == repeatPassword, "Passwords don't match");
        VERIFY(password.find(' ') == std::string::npos, "Password can't contain a space");
        VERIFY(password.length() > 5 && password.length() < 21, "Password must be at least 6 characters, and at most 20");

        Log("Sending Sign Up request");

        std::string msg = "/SignUp/";
        msg += login + ' ' + username + ' ' + password;
        Network::SendMessage(msg.c_str());
        
        signInRequested = true;

    }

}
