#include<iostream>

#include "Client.h"
#include "Chat.h"

using namespace std;

int main()
{
	std::setlocale(LC_ALL, "ru_RU.utf8");

	string clientMessage, serverMessage;

	Client client;
	Chat chat;

	client.createSocket();
    client.connectionToServer();

	while(1) {
		clientMessage = chat.chatProcessing();
        client.writeData(clientMessage);
		if (clientMessage == "exit") {
			break;
		}
		serverMessage = client.readData();
		chat.messageProcessing(serverMessage);
	}

	client.closeSocket();

	return 0;
}