#pragma once

#include <Core.h>

namespace Aluminium {

    class User {

    public:
        User() = default;
        User(uint32 connID, const std::string& name) : connID(connID), name(name) {}

        inline uint32 GetConnID() const { return connID; }
        inline const std::string& GetName() const { return name; }

        inline operator bool() const { return connID != INVALID_CONN_ID; }

    private:
        uint32 connID = INVALID_CONN_ID;
        std::string name = "";

    };

}
