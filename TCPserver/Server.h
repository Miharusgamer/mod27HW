#pragma once
#include<iostream>
#include<string.h>

#ifdef _WIN32
#include <winsock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#endif


using namespace std;

#define MESSAGE_LENGTH 1024
#define PORT 7777

class Server
{
public:
#ifdef _WIN32
    WSADATA wsaData;
    SOCKET sockert_file_descriptor, connection;
#else
    int sockert_file_descriptor, connection;
#endif

    struct sockaddr_in serveraddress, client;
    socklen_t length;
    int bind_status, connection_status;

    void createSocket();
    void bindSocket();
    void closeSocket();
    void dataReceivingServer();
    char* readData();
    void writeData(string serverMessage);

private:
    char message[MESSAGE_LENGTH];
};