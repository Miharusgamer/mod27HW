#include "Client.h"

using namespace std;


void Client::createSocket()
{
#ifdef _WIN32
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        cout << "Не удалось инициализировать Winsock!" << endl;
        exit(1);
    }

    socket_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_file_descriptor == INVALID_SOCKET)
    {
        cout << "Не удалось создать сокет!" << endl;
        exit(1);
    }
#else
    socket_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_file_descriptor == -1)
    {
        cout << "Не удалось создать сокет!" << endl;
        exit(1);
    }
#endif
}

void Client::connectionToServer() {
    serveraddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    serveraddress.sin_port = htons(PORT);
    serveraddress.sin_family = AF_INET;

#ifdef _WIN32
    connection = connect(socket_file_descriptor, (struct sockaddr*)&serveraddress, sizeof(serveraddress));

    if (connection == SOCKET_ERROR) {
        cout << "Не удалось установить соединение с сервером!" << endl;
        exit(1);
    }
#else   
    connection = connect(socket_file_descriptor, (struct sockaddr*)&serveraddress, sizeof(serveraddress));

    if (connection == -1) {
        cout << "Не удалось установить соединение с сервером!" << endl;
        exit(1);
    }
#endif
}

void Client::closeSocket()
{
#ifdef _WIN32
    closesocket(socket_file_descriptor);
    WSACleanup();
#else
    close(socket_file_descriptor);
#endif
}

char* Client::readData()
{
#ifdef _WIN32
    memset(message, '\0', MESSAGE_LENGTH);
    recv(socket_file_descriptor, message, sizeof(message), 0);
#else
    bzero(message, sizeof(message));
    read(socket_file_descriptor, message, sizeof(message));
#endif

    return message;
}

void Client::writeData(string clientMessage)
{
#ifdef _WIN32
    memset(message, '\0', MESSAGE_LENGTH);
    strcpy_s(message, clientMessage.c_str());

    send(socket_file_descriptor, message, sizeof(message), 0);
#else
    bzero(message, sizeof(message));
    strcpy_s(message, clientMessage.c_str());

    write(socket_file_descriptor, message, sizeof(message));
#endif
}
