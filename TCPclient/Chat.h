#pragma once
#include <string.h>
#include <vector>
#include <exception>
#include <memory>

using namespace std;


class Chat {
public:
    string chatProcessing();
    void messageProcessing(string serverMessage);

private:
    bool _sendMessage = false;
    bool _isCurrentUser = false;
    string _currentUserName;
    string _currentMenu = "";
    vector<string> _vectorFromMessage;

    void showLoginMenu();
    string loginUser();
    string registerUser();
    void checkRegister();
    void checkLogin();
    string exitChat();

    void showUserMenu();
    string getChat();
    string addMessage();
    string getUsers();
    void showChat();
    void checkAddMessage();
    void showUsers();

    void backMenu(string currentMenu);

    vector<string> messageToVector(string message, string delimiter);
};
