#include "Network.h"
#include "NetworkServer.h"
#include <string>
#include <iostream>
#include <unistd.h>             //базовые функции для работы с системой Linux
#include <sys/socket.h>         //для работы с сокетами
#include <netinet/in.h>         //содержит структуры и константы для работы с протоколами
#include <stdexcept>            //исключения




void NetworkServer::start() {
    //создает сокет ТСП
    _server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (_server_fd < 0)
        throw std::runtime_error("Сокет _server_fd не создан!");
    
    // Настройка конфигурации
    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(_port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    //привязать сокет к порту
    if (bind(_server_fd, (sockaddr*)&server_addr, sizeof(server_addr))) {
        close(_server_fd);
        throw std::runtime_error("Связь потеряна!");
    }

    //прослушиваем сокет (максимально подключений 5)
    if (listen(_server_fd, 5)) {
        close(_server_fd);
        throw  std::runtime_error("Ошибка при прослушивании сокета!");
    }

    std::cerr << "Сервер прослушивание на порту: " << _port << std::endl;

}


void NetworkServer::acceptClient()  {
    sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    
    // Сохраняем клиентский сокет в _client_socket
    _client_socket = accept(_server_fd, (sockaddr*)&client_addr, &addr_len);

    if (_client_socket < 0)
        throw std::runtime_error("Ошибка подключения клиента");
    
    std::cerr << "Клиент подключен к сокету: " << _client_socket << std::endl;
}


//метод читает все данные (в отличие от прим в модуле, где обрезается до '\n')
std::string NetworkServer::getMess() {
    char buffer[1024] = {0};

    int bytes_read = recv(_client_socket, buffer, sizeof(buffer), 0);

    if (bytes_read <= 0) 
        throw  std::runtime_error("Ошибка в получении сообщения или закрыто соединение");

    return std::string(buffer, bytes_read);
}


void NetworkServer::sendMess(const std::string& mess)  {
    int result = send(_client_socket, mess.c_str(), mess.size(), 0);
    if (result <= 0) {
        throw std::runtime_error("Ошибка отправки Mess");
    }
}
