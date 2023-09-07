#include <iostream>

#include "Server.h"
#include "Chat.h"

using namespace std;

int main()
{
    std::setlocale(LC_ALL, "ru_RU.utf8");

    string serverMessage, clientMessage;

    Server server;
    Chat chat;

    server.createSocket();
    server.bindSocket();
    server.dataReceivingServer();

    while(1) {
        clientMessage = server.readData();
        if (clientMessage == "exit") {
			break;
		}
        serverMessage = chat.messageProcessing(clientMessage);
        server.writeData(serverMessage);
    }

    server.closeSocket();

    return 0;
}
