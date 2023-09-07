#include <iostream>
#include <ranges>
#include <string_view>

#include "Chat.h"
#include "Database.h"
#include "Logger.h"

/* Основной метод, осуществляющий процессинг команд
 * Здесь происходит обработка сообщения, приходящего от клиента.
 * А также вызов соответствующего метода для формирование ответа для клиента.
 *
 * Сообщение от клиента приходит в виде:
 * <Название метода, отправившего сообщение>:<данные с разделителем через двоеточие>
 * Аналогичным образом формируется ответ клиенту.
 * Мы всегда знаем, какой метод клиента и что отправил на сервер, и наоборот.
 */
string Chat::messageProcessing(string clientMessage)
{
    string outMessage;

    _vectorFromMessage = messageToVector(clientMessage, ":");

    if (_vectorFromMessage[0] == "loginUser") {
        outMessage = loginUser();
    }
    else if (_vectorFromMessage[0] == "registerUser") {
        outMessage = registerUser();
    }
    else if (_vectorFromMessage[0] == "showChat") {
        outMessage = showChat();
    }
    else if (_vectorFromMessage[0] == "addMessage") {
        outMessage = addMessage();
    }
    else if (_vectorFromMessage[0] == "showUsers") {
        outMessage = showUsers();
    }

    return outMessage;
}

/**
 * Парсинг входящего сообщения.
 * Метод разбивает строку по разделителю на отдельные элементы
 * и записывает их в вектор.
 */
vector<string> Chat::messageToVector(string message, string delimiter)
{
    vector<string> resultVector;

    if (!message.empty()) {
        int start = 0;
        do {
            int idx = message.find(delimiter, start);
            if (idx == string::npos) {
                break;
            }

            int length = idx - start;
            resultVector.push_back(message.substr(start, length));
            start += (length + delimiter.size());
        } while (true);

        resultVector.push_back(message.substr(start));
    }

    return resultVector;
}

/**
 * Метод, формирующий ответ клиенту по попытке входа.
 * Если логин или пароль введены некорректно,
 * то клиент получает сообщение "error",
 * в случае верного ввода, клиент получает сообщение "ok".
*/
string Chat::loginUser()
{
    string login, password, name, outMessage;
    login = _vectorFromMessage[1];
    password = _vectorFromMessage[2];

    Database database{};
    database.connectDatabase(db_host, db_user, db_password, db_name);

    if (database.checkLogin(login, password)) {
        outMessage = "loginUser:ok:" + database.getName(login);
    }
    else {
        outMessage = "loginUser:error";
    }

    database.disconnect();

    return outMessage;
}

/**
 * Метод, формирующий ответ клиенту по попытке регистрации.
 * Если логин или имя введены некорректно,
 * то клиент получает сообщение "error",
 * в случае, если зарегистрирован уникальный пользователь,
 * то клиент получает сообщение "ok".
 *
 * Если пользователь уникальный, то его логин, пароль и имя
 * записываются в вектор _users класса User
*/


string Chat::registerUser()
{
    string login, password, name, outMessage;
    login = _vectorFromMessage[1];
    password = _vectorFromMessage[2];
    name = _vectorFromMessage[3];

    Database database{};
    database.connectDatabase(db_host, db_user, db_password, db_name);

    if (login == database.checkRegister(login, name, password) || login == "all") {
        outMessage = "registerUser:error:login";
        database.disconnect();
        return outMessage;
    }

    string nameExists = database.checkRegister(login, name, password);
    if (nameExists.empty()) {
        outMessage = "registerUser:error:name";
        database.disconnect();
        return outMessage;
    }

    if (database.registerUser(login, name, password)) {
        cout << "Зарегистрирован новый пользователь: " << login << endl;
        outMessage = "registerUser:ok:"+ name ;
    }
    else {
        // Error registering user
        outMessage = "registerUser:error:registration_failed";
    }

    database.disconnect();

    return outMessage;

}

/**
 * Метод, формирующий ответ клиенту, получилось отправить сообщение в чат или нет.
 * Если сообщение отправлено в чат, то оно записывается в вектор _messages класса Message.
*/
string Chat::addMessage()
{
    string from, to, text, outMessage;
    from = _vectorFromMessage[1];
    to = _vectorFromMessage[2];
    text = _vectorFromMessage[3];

    Database database{};
    database.connectDatabase(db_host, db_user, db_password, db_name);

    Logger logger("log.txt");
    logger.writeLog("from: " + from + " to: " + to + " message: " + text);

    thread readerThread([&logger]() {
        string lastLine = logger.readLog();
        cout << "Last Line from Log: " << lastLine << "\n";
        });

    readerThread.join();

    if (database.addMessage(from, to, text)) {
        outMessage = "addMessage:ok";
    }
    else {
        outMessage = "addMessage:error:" + to;
    }

    database.disconnect();

    return outMessage;
}

/**
 * Метод, формирующий список всех сообщений отправленных в чат всеми пользователями.
 * Если чат не пустой то формируется сообщение клиенту вида:
 * <название метода: в данном случае showChat>:
 * <от кого:пользователь 1>:<кому>:<текст>:
 * <от кого:пользователь 2>:<кому>:<текст>:
 * .....
 * <от кого:пользователь N>:<кому>:<текст>:
 * <название метода: в данном случае showChat>
 * (посленее нужно чтобы при парсинге не получить отсутствующий символ после разделителя)
*/
string Chat::showChat()
{
    string from, to, text, outMessage, name;
    name = _vectorFromMessage[1];

    Database database{};
    database.connectDatabase(db_host, db_user, db_password, db_name);
    
    vector<string> messages = database.getAllMessages(name);

    outMessage = "showChat:";

    for (const auto& message : messages) {
        outMessage += message + ":";
    }

    outMessage += "showChat";

    database.disconnect();

    return outMessage;
}

/**
 * Метод, формирующий список всех зарегистрированных пользователей.
 * Если чат не пустой то формируется сообщение клиенту вида:
 * <название метода: в данном случае showUsers>:
 * <имя пользователя 1>:
 * <имя пользователя 2>:
 * .....
 * <имя пользователя N>:
 * <название метода: в данном случае showUsers>
 * (посленее нужно чтобы при парсинге не получить отсутствующий символ после разделителя)
*/
string Chat::showUsers()
{
    string name, outMessage;
    name = _vectorFromMessage[1];

    Database database{};
    database.connectDatabase(db_host, db_user, db_password, db_name);
    
    vector<string> users = database.getAllUsers();
    
    outMessage = "showUsers:";

    for (const auto& user : users) {
        if (user == name) {
            outMessage += user + "(я):";
        }
        else {
            outMessage += user + ":";
        }
    }

    outMessage += "showUsers";

    database.disconnect();

    return outMessage;
}
