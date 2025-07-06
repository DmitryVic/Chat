#include <iostream>
#include <unistd.h>             //базовые функции для работы с системой Linux
#include <string.h>             //библиотека для работы со строками C
#include <sys/socket.h>         //для работы с сокетами
#include <netinet/in.h>         //содержит структуры и константы для работы с протоколами
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
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (sockaddr*)&server_addr, sizeof(server_addr))) {
        cerr << "Связь потеряна!" << endl;
        close(sockfd);
        return 1;
    }

    if (listen(sockfd, 5)) {
        cerr << "Ошибка при прослушивании сокета!" << endl;
        close(sockfd);
        return 1;
    }

    cout << "Сервер прослушивание на порту" << PORT << endl;

    sockaddr_in client_addr{};
    socklen_t client_len = sizeof(client_addr);
    int client_sock = accept(sockfd, (sockaddr*)&client_addr, &client_len);
    if (client_sock < 0) {
        cerr << "Не удалось принять сообщение!" << endl;
        close(sockfd);
        return 1;
    }

    char buffer[BUFFER_SIZE];
    while (true) {
        memset(buffer, 0, BUFFER_SIZE);
        ssize_t bytes_received = recv(client_sock, buffer, BUFFER_SIZE - 1, 0);
        if (bytes_received <= 0) {
            cerr << "Ошибка в получении сообщения или закрыто соединение" << endl;
            break;
        }

        try {
            // Парсим JSON запрос
            json request = json::parse(buffer);
            cout << "Полученный: " << request.dump(4) << endl;

            // Обрабатываем запрос
            json response;
            if (request["type"] == 0) {
                // Аутентификация
                response["type"] = 1;
                response["login"] = request["login"];
                response["success"] = (request["password"] == "pass123");
            }
            // Добавить обработку других типов сообщений

            // Отправляем ответ
            string json_str = response.dump();
            send(client_sock, json_str.c_str(), json_str.size(), 0);

        } catch (const exception& e) {
            cerr << "Ошибка распаковки JSON: " << e.what() << endl;
            json error_response;
            error_response["type"] = 8;
            error_response["error"] = "Ошибка JSON формата";
            string error_str = error_response.dump();
            send(client_sock, error_str.c_str(), error_str.size(), 0);
        }
        break;
    }

    close(client_sock);
    close(sockfd);
    return 0;
}