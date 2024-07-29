#include "Database.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/statement.h>
#include <mysql_connection.h>
#include <mysql_driver.h>

#define SQL_USER "dev"

#define BEGIN try {
#define END } catch(sql::SQLException& e) { LogError("An exception was caught during a database call.\n{}", e.what()); }

namespace Aluminium::Database {

    sql::Connection* conn = nullptr;
    sql::Statement* cmd = nullptr;

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

    void AddUser(const char *email, const char *username, const char *password) {

        std::string text = "INSERT INTO Users(email, name, password) VALUES('";
        text += email; text += "','";
        text += username; text += "','";
        text += password; text += "');";
        cmd->execute(text);

    }

}
