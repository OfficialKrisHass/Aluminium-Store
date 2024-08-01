#pragma once

#include <Core.h>

namespace Aluminium { class User; }

namespace Aluminium::Database {

    void Initialize(const char* pass);
    void Shutdown();

    void AddUser(User& out, const std::string& email, const std::string& username, const std::string& salt, const std::string& password);

    void GetUser(User& out, uint32 id);
    void GetUser(User& out, const std::string& login);

    void GetUserSalt(const std::string& login, std::string& out);

    bool CheckUserPassword(uint32 id, const std::string& password);

}
