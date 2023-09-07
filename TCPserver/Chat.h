#pragma once
#include <vector>
#include <exception>
#include <memory>

#include "Message.h"
#include "User.h"

using namespace std;

class Chat {
public:
    string messageProcessing(string clientMessage);

private:
    vector<string> _vectorFromMessage;

    string loginUser();
    string registerUser();
    string showChat();
    string showUsers();
    string addMessage();

    vector<string> messageToVector(string message, string delimiter);
};