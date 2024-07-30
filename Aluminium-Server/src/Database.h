#pragma once

#include <Core.h>

namespace Aluminium::Database {

    void Initialize(const char* pass);
    void Shutdown();

    void AddUser(const char* email, const char* username, const char* password);
    bool GetUserFromEmail(const char* email);
    bool GetUserFromName(const char* username);
    bool CheckUserPassword(const char* login, const char* password);

}
