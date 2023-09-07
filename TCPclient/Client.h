#pragma once
#include <iostream>
#include <string>

#ifdef _WIN32
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#endif


using namespace std;

#define MESSAGE_LENGTH 1024
#define PORT 7777

class Client
{
public:
#ifdef _WIN32
    WSADATA wsaData;
    SOCKET socket_file_descriptor, connection;
#else
    int socket_file_descriptor, connection;
#endif
    struct sockaddr_in serveraddress, client;

    void createSocket();
    void connectionToServer();
    void closeSocket();
    char* readData();
    void writeData(string clientMessage);

private:
    char message[MESSAGE_LENGTH];
};
