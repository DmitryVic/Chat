#pragma once
#include "Network.h"
#include <string>
#include <iostream>
#include <unistd.h>             //базовые функции для работы с системой Linux
#include <sys/socket.h>         //для работы с сокетами
#include <netinet/in.h>         //содержит структуры и константы для работы с протоколами
#include <stdexcept>            //исключения



class NetworkServer : public Network
{
private:
    int _server_fd;         // дескриптор серверного сокета
    int _port;
    int _client_socket;     // дескриптор клиенского сокета
    
public:
    
    explicit NetworkServer(int port) : _port(port), _server_fd(-1), _client_socket(-1) {};

    ~NetworkServer() {
        if (_client_socket != -1)
        {
            close(_client_socket);
        }
        if (_server_fd != -1)
        {
            close(_server_fd);
        }
        
    };
    
    void start() override;


    void acceptClient() override ;


    //метод читает все данные (в отличие от прим в модуле, где обрезается до '\n')
    std::string getMess() override;


    void sendMess(const std::string& mess) override;
};