#pragma once

#include <Core.h>

namespace Aluminium::Database {

    void Initialize(const char* pass);
    void Shutdown();

    void AddUser(const char* email, const char* username, const char* password);

}
