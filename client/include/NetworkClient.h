#pragma once
#include <string>
#include <iostream>
#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib") // Подключаем библиотеку Winsock
    typedef SOCKET socket_t;         // Определяем тип сокета для Windows
#else
    #include <unistd.h>             //базовые функции для работы с системой Linux
    #include <sys/socket.h>         //для работы с сокетами
    #include <netinet/in.h>         //содержит структуры и константы для работы с протоколами
    #include <stdexcept>            //исключения
    #include <arpa/inet.h>          // преобразовать  ip inet_pton()
    typedef int socket_t;           // Определяем тип сокета для Linux
#endif


class NetworkClient {
private:
    socket_t sock;              // Единственный дескриптор сокета для общения с сервером
    std::string server_ip; // IP-адрес сервера для подключения
    int port;              // Порт сервера для подключения

public:
    // Конструктор: только инициализация данных (сокет еще не создан)
    NetworkClient(const std::string& ip, int port);
    
    ~NetworkClient();

    // Метод подключения к серверу
    void connecting();

    // ОТПРАВКА СООБЩЕНИЯ
    void sendMess(const std::string& message);

    // ПОЛУЧЕНИЕ СООБЩЕНИЯ
    std::string getMess();

};