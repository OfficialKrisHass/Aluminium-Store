#pragma once

#include <Core.h>

namespace Aluminium {

    class User {

    public:
        User() = default;
        User(uint32 id, const std::string& email, const std::string& name)
            : id(id), email(email), name(name) {}

        inline uint32 GetID() const { return id; }

        inline const std::string& GetEmail() const { return email; }
        inline const std::string& GetName() const { return name; }

        inline operator bool() const { return id != 0; }

    private:
        uint32 id = 0;

        std::string email = "";
        std::string name = "";

    };

}
