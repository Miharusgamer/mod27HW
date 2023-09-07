#pragma once
#include <iostream>
#include <mysql.h>
#include <string.h>
#include <vector>

using namespace std;

constexpr auto db_host = "localhost";
constexpr auto db_user = "root";
constexpr auto db_password = "9568";
constexpr auto db_name = "chat";

class Database {
public:
    MYSQL mysql;
    MYSQL_RES* res;
    MYSQL_ROW row;

    bool connectDatabase(const string& host, const string& user, const string& password, const string& dbName);
    void disconnect();

    bool checkLogin(string login, string password);
    string getName(string login);

    string checkRegister(string login, string name, string password);
    bool registerUser(string login, string name, string password);

    bool addMessage(string from, string to, string message);
    vector<string> getAllMessages(string login);
    vector<string> getAllUsers();

private:
    vector<string> convertResToVector(MYSQL_RES* res);
};
