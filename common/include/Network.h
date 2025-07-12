#pragma once 
#include <string>
#include <iostream>
#include <unistd.h>             //базовые функции для работы с системой Linux
#include <sys/socket.h>         //для работы с сокетами
#include <netinet/in.h>         //содержит структуры и константы для работы с протоколами
#include <stdexcept>            //исключения


class Network
{
private:

public:
    Network(){};
    virtual ~Network() = default;
    virtual void start() = 0;
    virtual void acceptClient() = 0;
    virtual std::string getMess() = 0;
    virtual void sendMess(const std::string& message) = 0;
};
