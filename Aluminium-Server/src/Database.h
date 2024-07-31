#pragma once

#include <Core.h>

namespace Aluminium { class User; }

namespace Aluminium::Database {

    void Initialize(const char* pass);
    void Shutdown();

    User AddUser(const std::string& email, const std::string& username, const std::string& salt, const std::string& password);

    User GetUser(uint32 id);
    User GetUser(const std::string& login);

    void GetUserSalt(const std::string& login, std::string& out);

    bool CheckUserPassword(uint32 id, const std::string& password);

}
