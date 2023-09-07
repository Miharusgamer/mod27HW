#include "Database.h"
#include <iostream>
#include <string>


bool Database::connectDatabase(
    const string& host,
    const string& user,
    const string& password,
    const string& dbName)
{
    // Получаем дескриптор соединения
    mysql_init(&mysql);

    if (&mysql == nullptr) {
        cout << "Ошибка инициализации соединения с базой данных" << endl;
        exit(1);
    }

    // Подключаемся к серверу
    if (!mysql_real_connect(&mysql, host.c_str(), user.c_str(), password.c_str(), dbName.c_str(), 0, nullptr, 0)) {
        // Если нет возможности установить соединение с БД выводим сообщение об ошибке
        cout << "Ошибка подключения к базе данных: " << mysql_error(&mysql) << endl;
        return false;
    }
    else {
        // Если соединение успешно установлено выводим сообщение
        cout << "Подключение к базе данных выполнено успешно!" << endl;
    }

    mysql_set_character_set(&mysql, "utf8mb4");
    cout << "connection characterset: " << mysql_character_set_name(&mysql) << endl;
    return true;
}


void Database::disconnect() {
    // Закрываем соединение с сервером базы данных
    mysql_close(&mysql);
}

bool Database::checkLogin(string login, string password) {
    string query = "SELECT passwords.password_hash FROM users "
        "JOIN passwords ON users.user_id = passwords.user_id "
        "WHERE users.login = '" + login + "'";

    if (mysql_query(&mysql, query.c_str()) == 0) {
        res = mysql_store_result(&mysql);
        if (mysql_fetch_row(res) != nullptr) {
            mysql_free_result(res);
            return true;
        }
    }

    return false;
}


string Database::getName(string login) {
    string getName = "SELECT name FROM users "
        "WHERE login = '" + login + "'";

    string name;

    if (mysql_query(&mysql, getName.c_str()) == 0) {
        res = mysql_store_result(&mysql);
        if (row = mysql_fetch_row(res)) {
            name = row[0];
            mysql_free_result(res);

            return name;
        }
    }

    return "";
}


string Database::checkRegister(string login, string name, string password) {
    string getLogin = "SELECT login FROM users "
        "WHERE login = '" + login + "'";

    string getName = "SELECT name FROM users "
        "WHERE name = '" + name + "'";

    if (mysql_query(&mysql, getLogin.c_str()) == 0) {
        res = mysql_store_result(&mysql);
        if (mysql_fetch_row(res) != nullptr) {
            mysql_free_result(res);
            return login;
        }
    }
    else if (mysql_query(&mysql, getName.c_str()) == 0) {
        res = mysql_store_result(&mysql);
        if (mysql_fetch_row(res) != nullptr) {
            mysql_free_result(res);
            return name;
        }
    }

    return "";
}


bool Database::registerUser(string login, string name, string password) {
    string registerUserQuery = "INSERT INTO users (name, login) "
        "VALUES ('" + name + "', '" + login + "')";

    // Добавляем пароль с использованием подзапроса
    string addUserPasswordQuery =
        "INSERT INTO passwords (user_id,password_hash)\n"
        "VALUES ((SELECT LAST_INSERT_ID()), '" + password + "')";

    if (mysql_query(&mysql, registerUserQuery.c_str()) == 0 &&
        mysql_query(&mysql, addUserPasswordQuery.c_str()) == 0) {
        return true;
    }

    return false;
}

bool Database::addMessage(string from, string to, string message) {
    // Получаем идентификатор отправителя (from_id)
    string getFromId = "SELECT user_id FROM users WHERE name = '" + from + "'";
    if (mysql_query(&mysql, getFromId.c_str()) == 0) {
        res = mysql_store_result(&mysql);
        if (row = mysql_fetch_row(res)) {
            int fromId = atoi(row[0]);
            mysql_free_result(res);

            // Получаем идентификатор получателя (to_id)
            string getToId = "SELECT user_id FROM users WHERE name = '" + to + "'";
            if (mysql_query(&mysql, getToId.c_str()) == 0) {
                res = mysql_store_result(&mysql);
                if (row = mysql_fetch_row(res)) {
                    int toId = atoi(row[0]);
                    mysql_free_result(res);

                    // Вставляем сообщение в таблицу messages
                    string addMsg =
                        "INSERT INTO messages (from_id, to_id, message) "
                        "VALUES ('" + to_string(fromId) + "', '" + to_string(toId) + "', '" + message + "')";
                    if (mysql_query(&mysql, addMsg.c_str()) == 0) {
                        return true;
                    }
                }
            }
        }
    }

    return false;
}


vector<string> Database::getAllMessages(string name) {
    string getAllMessages = "SELECT u1.name AS from_name, u2.name AS to_name, m.message "
        "FROM messages m "
        "JOIN users u1 ON u1.user_id = m.from_id "
        "JOIN users u2 ON u2.user_id = m.to_id "
        "WHERE(u1.name = '" + name + "' OR u2.name = '" + name + "') OR m.to_id = 1";

    vector<string> resultVector;

    if (mysql_query(&mysql, getAllMessages.c_str()) == 0) {
        res = mysql_store_result(&mysql);
        resultVector = convertResToVector(res);
        mysql_free_result(res);

        return resultVector;
    }

    return vector<string>();
}


vector<string> Database::getAllUsers() {
    string getAllUsers = "SELECT name FROM users";
    vector<string> resultVector;

    if (mysql_query(&mysql, getAllUsers.c_str()) == 0) {
        res = mysql_store_result(&mysql);
        resultVector = convertResToVector(res);
        mysql_free_result(res);

        return resultVector;
    }
    
    return vector<string>();
}


vector<string> Database::convertResToVector(MYSQL_RES* res) {
    vector<string> resultVector;

    while (row = mysql_fetch_row(res)) {
        for (int i = 0; i < mysql_num_fields(res); i++) {
            if (row[i] != nullptr) {
                resultVector.push_back(string(row[i]));
            }
        }
    }

    return resultVector;
}
