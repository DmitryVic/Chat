#pragma once
#include <string>
#include <iostream>
#include <unistd.h>             //базовые функции для работы с системой Linux
#include <sys/socket.h>         //для работы с сокетами
#include <netinet/in.h>         //содержит структуры и константы для работы с протоколами
#include <stdexcept>            //исключения
#include <arpa/inet.h>          // преобразовать  ip inet_pton()


class NetworkClient {
private:
    int sock;              // Единственный дескриптор сокета для общения с сервером
    std::string server_ip; // IP-адрес сервера для подключения
    int port;              // Порт сервера для подключения

public:
    // Конструктор: только инициализация данных (сокет еще не создан)
    NetworkClient(const std::string& ip, int port) 
        : server_ip(ip), port(port), sock(-1) {}  // -1 = сокет не инициализирован
    
    ~NetworkClient() {
        if (sock != -1) {
            close(sock);
            std::cerr << "Соединение с сервером закрыто" << std::endl;
        }
    }

    // Метод подключения к серверу
    void connecting();

    // ОТПРАВКА СООБЩЕНИЯ
    void sendMess(const std::string& message);

    // ПОЛУЧЕНИЕ СООБЩЕНИЯ
    std::string getMess();

};