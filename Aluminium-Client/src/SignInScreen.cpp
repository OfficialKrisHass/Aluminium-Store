#include "SignInScreen.h"
#include "Network.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <misc/cpp/imgui_stdlib.h>

#include <argon2.h>
#include <argon2-core.h>

#include <ctime>
#include <cstdlib>

#define HASH_LEN 32
#define SALT_LEN 48
#define MEMORY 19922
#define ITERATIONS 2
#define THREADS 1

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

    void HashPassword(std::string& password, const std::string& salt);

    void ClearInput();

    void UI(bool* signedIn, uint32* userID) {

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

        Network::Message response;
        Network::RecieveMessage(&response);
        if (!response) return;

        if (response.type == MESSAGE_USER_SALT) {

            Log("Salt recieved, sending Sign in request");

            std::string hashedPassword = password;
            std::string salt = response.msg; 
            HashPassword(hashedPassword, salt);

            Network::SendMessage(MESSAGE(MESSAGE_SIGNIN) + login + ' ' + hashedPassword);
            return;

        }

        ClearInput();
        if (response.type == MESSAGE_SIGNIN_FAIL) {

            errMsg = "Failed to ";
            errMsg += (signUp ? "Sign Up: " : "Sign In: ");
            errMsg += response.msg;

            LogError(errMsg.c_str());
            return;

        }

        *signedIn = true;
        *userID = atoi(response.msg);
        Log("Successfully signed In with userID {}", *userID);


    }

    void SignIn() {

        ImGui::InputText("Username or email: ", &login);
        ImGui::InputText("Password: ", &password);

        if (!ImGui::Button("Sign In")) return;

        Log("Sending User salt request");

        Network::SendMessage(MESSAGE(MESSAGE_USER_SALT) + login);
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
        VERIFY(username.length() < 33, "Username can't be longer than 32 characters");

        VERIFY(login.find('@') != std::string::npos, "Invalid email");
        VERIFY(login.find(' ') == std::string::npos, "Email can't contain a space");
        VERIFY(login.length() < 256, "Email can't be longer than 255 characters");

        VERIFY(password == repeatPassword, "Passwords don't match");
        VERIFY(password.find(' ') == std::string::npos, "Password can't contain a space");
        VERIFY(password.length() > 5 && password.length() < 21, "Password must be at least 6 characters, and at most 20");

        std::string hashedPassword = password;
        std::string salt = username + std::to_string(time(0));
        if (salt.length() > SALT_LEN)
            salt.resize(SALT_LEN);

        HashPassword(hashedPassword, salt);

        Network::SendMessage(MESSAGE(MESSAGE_SIGNUP) + login + ' ' + username + ' ' + salt + ' ' + hashedPassword);
        signInRequested = true;

    }
    
    void HashPassword(std::string& password, const std::string& salt) {

        // Hash

        uint8 out[HASH_LEN];
        Argon2_Context context = Argon2_Context(out, HASH_LEN, (uint8*) password.data(), password.length(), (uint8*) salt.data(), salt.length(), 
                nullptr, 0, nullptr, 0, 
                ITERATIONS, MEMORY, 1, THREADS, 
                nullptr, nullptr, true, false, false, false);

        int32 ret = Argon2id(&context);
        AL_ASSERT(ret == ARGON2_OK, "Could not hash the password");

        // Convert to hex

        static const char hexTable[] = "0123456789ABCDEF";

        password.resize(HASH_LEN * 2, '0');
        for (uint32 i = 0; i < HASH_LEN; i++) {

            password[i * 2] = hexTable[out[i] >> 4];
            password[i * 2 + 1] = hexTable[out[i] & 0xF];

        }

    }

    void ClearInput() {

        signInRequested = false;
        login.clear();
        password.clear();
        
        if (!signUp) return;

        username.clear();
        repeatPassword.clear();

    }

}
