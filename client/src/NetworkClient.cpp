#include <string>
#include <iostream>
#include <unistd.h>             //базовые функции для работы с системой Linux
#include <sys/socket.h>         //для работы с сокетами
#include <netinet/in.h>         //содержит структуры и константы для работы с протоколами
#include <stdexcept>            //исключения
#include <arpa/inet.h>          // преобразовать  ip inet_pton()
#include "NetworkClient.h"


// Метод подключения к серверу
void NetworkClient::connecting() {
    // СОЗДАНИЕ СОКЕТА
    // AF_INET = IPv4, SOCK_STREAM = TCP, 0 = протокол по умолчанию
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        throw std::runtime_error("Не удалось создать сокет");
    }

    // НАСТРОЙКА АДРЕСА СЕРВЕРА
    sockaddr_in serv_addr;               // Структура для адреса сервера
    serv_addr.sin_family = AF_INET;      // Семейство адресов - IPv4
    
    // Преобразование порта в сетевой порядок байт (big-endian)
    serv_addr.sin_port = htons(port);    
    
    // Преобразование IP-адреса из строки в бинарный формат
    // inet_pton (IP presentation to network)
    if (inet_pton(AF_INET, server_ip.c_str(), &serv_addr.sin_addr) <= 0) {
        close(sock);  // Закрываем сокет при ошибке
        throw std::runtime_error("Некорректный адрес сервера");
    }

    // ПОДКЛЮЧЕНИЕ К СЕРВЕРУ
    // connect() устанавливает соединение с указанным адресом
    // sizeof(serv_addr) - размер структуры адреса
    if (connect(sock, (sockaddr*)&serv_addr, sizeof(serv_addr))) {
        close(sock);
        throw std::runtime_error("Ошибка подключения");
    }
}

// ОТПРАВКА СООБЩЕНИЯ
void NetworkClient::sendMess(const std::string& message) {
    // send() записывает данные в сокет
    // message.c_str() - указатель на данные
    // message.size() - длина данных в байтах
    // 0 - флаги (по умолчанию)
    std::cerr << "NetworkClient::sendMess MESS: " << message << "\n";
    if (send(sock, message.c_str(), message.size(), 0) < 0) {
        throw std::runtime_error("Ошибка отправки");
    }
}

// ПОЛУЧЕНИЕ СООБЩЕНИЯ
std::string NetworkClient::getMess() {
    char buffer[1024] = {0};  // Буфер для приема данных
    
    // recv() читает данные из сокета
    // sizeof(buffer) - максимальный размер данных для чтения
    int bytes_read = recv(sock, buffer, sizeof(buffer), 0);
    
    if (bytes_read < 0) throw std::runtime_error("Ошибка чтения");
    if (bytes_read == 0) throw std::runtime_error("Сервер закрыл соединение");
    
    // Важно! Создаем строку с указанием длины (bytes_read)
    // чтобы корректно обработать данные с нулевыми байтами
    return std::string(buffer, bytes_read);
}