#include "Server.h"

void Server::createSocket()
{
#ifdef _WIN32
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cout << "Ошибка инициализации Winsock: " << WSAGetLastError() << endl;
        exit(1);
    }

    sockert_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (sockert_file_descriptor == INVALID_SOCKET) {
        cout << "Не удалось создать сокет: " << WSAGetLastError() << endl;
        WSACleanup();
        exit(1);
    }
    else {
        cout << "Создание сокета прошло успешно!" << endl;
    }
#else
    sockert_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (sockert_file_descriptor == -1) {
        perror("Не удалось создать сокет");
        exit(1);
    }
    else {
        cout << "Создание сокета прошло успешно!" << endl;
    }
#endif
}

void Server::bindSocket()
{
    serveraddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddress.sin_port = htons(PORT);
    serveraddress.sin_family = AF_INET;

#ifdef _WIN32
    bind_status = bind(sockert_file_descriptor, (struct sockaddr*)&serveraddress, sizeof(serveraddress));
    if (bind_status == SOCKET_ERROR) {
        cout << "Ошибка привязки сокета: " << WSAGetLastError() << endl;
        exit(1);
    }
#else
    bind_status = bind(sockert_file_descriptor, (struct sockaddr*)&serveraddress, sizeof(serveraddress));
    if (bind_status == -1) {
        perror("Ошибка привязки сокета");
        exit(1);
    }
#endif

    cout << "Привязка сокета прошла успешно!" << endl;
}

void Server::closeSocket()
{
#ifdef _WIN32
    closesocket(sockert_file_descriptor);
    WSACleanup();
#else
    close(sockert_file_descriptor);
#endif 
}

void Server::dataReceivingServer()
{
    connection_status = listen(sockert_file_descriptor, 5);

    if (connection_status == -1) {
#ifdef _WIN32
        cout << "Сокет не может прослушивать новые соединения: " << WSAGetLastError() << endl;
#else
        perror("Сокет не может прослушивать новые соединения");
#endif
        exit(1);
    }
    else {
        cout << "Сервер ожидает нового соединения: " << endl;
    }

    length = sizeof(client);
    connection = accept(sockert_file_descriptor, (struct sockaddr*)&client, &length);

    if (connection == -1) {
#ifdef _WIN32
        cout << "Сервер не может принять данные от клиента: " << WSAGetLastError() << endl;
#else
        perror("Сервер не может принять данные от клиента");
#endif
        exit(1);
    }
    else {
        cout << "Клиент присоединился!" << endl;
    }
}

char* Server::readData()
{
    cout << " >> Ожидание данных!" << endl;

#ifdef _WIN32
    memset(message, '\0', MESSAGE_LENGTH);

    int bytesReceived = recv(connection, message, sizeof(message), 0);
    if (bytesReceived == SOCKET_ERROR) {
        cout << "Ошибка при чтении данных от клиента: " << WSAGetLastError() << endl;
        exit(1);
    }
#else
    bzero(message, MESSAGE_LENGTH);

    int bytesRead = read(connection, message, sizeof(message));
    if (bytesRead == -1) {
        perror("Ошибка при чтении данных от клиента");
        exit(1);
    }
#endif

    cout << "Данные получены от клиента! << " << message << endl;
    return message;
}

void Server::writeData(string serverMessage)
{
#ifdef _WIN32
    memset(message, '\0', MESSAGE_LENGTH);
    strcpy_s(message, serverMessage.c_str());

    int bytesSent = send(connection, message, sizeof(message), 0);

    if (bytesSent == SOCKET_ERROR) {
        cout << "Ошибка отправки данных клиенту: " << WSAGetLastError() << endl;
        closesocket(connection);
        WSACleanup();
        exit(1);
    }
    else {
        cout << " >> Данные отправлены клиенту! >> " << message << endl;
    }
#else
    bzero(message, MESSAGE_LENGTH);
    strncpy(message, serverMessage.c_str(), MESSAGE_LENGTH - 1);

    ssize_t bytes = write(connection, message, sizeof(message));

    if (bytes == -1) {
        perror("Ошибка отправки данных клиенту");
        close(connection);
        exit(1);
    }
    else {
        cout << " >> Данные отправлены клиенту! >> " << message << endl;
    }
#endif
}
