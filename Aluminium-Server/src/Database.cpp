#include "Database.h"
#include "User.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/statement.h>
#include <mysql_connection.h>
#include <mysql_driver.h>

#define SQL_USER "dev"

#define BEGIN try {
#define END } catch(sql::SQLException& e) { LogError("An exception was caught during a database call.\n{}", e.what()); }

namespace Aluminium::Database {

    static sql::Connection* conn = nullptr;
    static sql::Statement* cmd = nullptr;
    static std::string cmdText = "";

    void Initialize(const char* pass) {

        BEGIN;
        Log("Initializing the Aluminium database");

        sql::mysql::MySQL_Driver* driver = sql::mysql::get_mysql_driver_instance();
        conn = driver->connect("tcp://localhost:3306", SQL_USER, pass);
        conn->setSchema("aluminium_store");

        cmd = conn->createStatement();

        Log("Successfully initialized Aluminium database");
        END;

    }
    void Shutdown() {

        Log("Shutting down the Aluminium database");

        delete conn;
        delete cmd;

    }

    User AddUser(const std::string& email, const std::string& username, const std::string& salt, const std::string& password) {

        BEGIN;

        cmdText = "INSERT INTO Users(email, name, salt, password) VALUES('";
        cmdText += email + "','" + username + "','" + salt + "','" + password + "');";
        cmd->execute(cmdText);

        cmdText = "SELECT id, name FROM Users WHERE name = '" + username + "';";
        sql::ResultSet* set = cmd->executeQuery(cmdText);

        if (!set->next()) {

            LogError("Could not get user after signing up");
            return User();

        }
        User ret = User(set->getInt("id"), email, username);

        delete set;
        return ret;

        END;
        return User();

    }
    User GetUser(uint32 id) {

        BEGIN;

        cmdText = "SELECT id, email, name FROM Users WHERE id = " + std::to_string(id) + ';';
        sql::ResultSet* set = cmd->executeQuery(cmdText);
        if (!set->next()) return User();

        User ret = User(set->getInt("id"), set->getString("email"), set->getString("name"));

        delete set;
        return ret;

        END;
        return User();

    }
    User GetUser(const std::string& login) {

        BEGIN;

        cmdText = "SELECT id, email, name FROM Users WHERE email = '" + login + "' OR name = '" + login + "';";
        sql::ResultSet* set = cmd->executeQuery(cmdText);
        if (!set->next()) return User();

        User ret = User(set->getInt("id"), set->getString("email"), set->getString("name"));

        delete set;
        return ret;

        END;
        return User();

    }

    void GetUserSalt(const std::string &login, std::string &out) {

        BEGIN;

        cmdText = "SELECT email, name, salt FROM Users WHERE email = '" + login + "' OR name = '" + login + "';";
        sql::ResultSet* set = cmd->executeQuery(cmdText);
        if (!set->next()) return;

        out = set->getString("salt");

        END;

    }

    bool CheckUserPassword(uint32 id, const std::string& password) {

        BEGIN;

        cmdText = "SELECT id, password FROM Users WHERE id = " + std::to_string(id) + " AND password = '" + password + "';";
        sql::ResultSet* set = cmd->executeQuery(cmdText);

        bool ret = set->next();

        delete set;
        return ret;

        END;
        return false;

    } 

}
