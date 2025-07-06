#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

#define BUFFER_SIZE 1024
#define PORT 7777

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        cerr << "Создание сокета не удалось!" << endl;
        return 1;
    }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sockfd, (sockaddr*)&server_addr, sizeof(server_addr))) {
        cerr << "Соединение не удалось!" << endl;
        close(sockfd);
        return 1;
    }

    char buffer[BUFFER_SIZE];
    while (true) {
        // Создаем JSON сообщение
        json message;
        message["type"] = 0; // Пример типа сообщения
        message["login"] = "user123";
        message["password"] = "pass123";
        
        // Преобразуем в строку
        string json_str = message.dump();
        
        // Отправляем
        if (send(sockfd, json_str.c_str(), json_str.size(), 0) <= 0) {
            cerr << "Отправить не удастся" << endl;
            break;
        }

        // Принимаем ответ
        memset(buffer, 0, BUFFER_SIZE);
        ssize_t bytes_received = recv(sockfd, buffer, BUFFER_SIZE - 1, 0);
        if (bytes_received <= 0) {
            cerr << "Получение не удалось" << endl;
            break;
        }

        try {
            // Парсим JSON ответ
            json response = json::parse(buffer);
            cout << "Серверный ответ: " << response.dump(4) << endl;
            
            if (response.contains("type") && response["type"] == 1) {
                if (response["success"] == true) {
                    cout << "Аутентификация успешно!" << endl;
                } else {
                    cout << "Аутентификация не удалась!" << endl;
                }
            }
        } catch (const exception& e) {
            cerr << "Ошибка распаковки JSON: " << e.what() << endl;
        }
        break;
    }

    close(sockfd);
    return 0;
}