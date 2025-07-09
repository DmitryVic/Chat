#include "Protocol.h"
#include "Network.h"
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
        
        //Формируем сообщение
        Message2 mess_class;
        mess_class.login = "LOGIN";
        mess_class.pass = "пароль";
        mess_class.name = "Дмитрий";

        // создаем json и преобразуем сообщение
        json mess_json;
        mess_class.to_json(mess_json);

        //json в строку
        string mess_push = mess_json.dump();
        
        
        // Отправляем
        if (send(sockfd, mess_push.c_str(), mess_push.size(), 0) <= 0) {
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


        // Полученый ответ передам на логику дальше
        std::string response = buffer;
        handle_incoming_message(response);

        //----------------------------------------------------------------------------------------------------------------------

        //Формируем сообщение
        Message3 mess_Pr;
        mess_Pr.user_sender = "LOGIN";
        mess_Pr.user_recipient = "Log";
        mess_Pr.mess = "Привет это сообщение только тебе!";

        mess_json.clear();  // Очищаем json перед новым использованием
        // создаем json и преобразуем сообщение
        mess_Pr.to_json(mess_json);

        //json в строку
        mess_push = mess_json.dump();
        
        
        // Отправляем
        if (send(sockfd, mess_push.c_str(), mess_push.size(), 0) <= 0) {
            cerr << "Отправить не удастся" << endl;
            break;
        }

        // Принимаем ответ
        memset(buffer, 0, BUFFER_SIZE);
        bytes_received = recv(sockfd, buffer, BUFFER_SIZE - 1, 0); // тут зависло
        if (bytes_received <= 0) {
            cerr << "Получение не удалось" << endl;
            break;
        }


        // Полученый ответ передам на логику дальше
        response = buffer;
        handle_incoming_message(response);


        //----------------------------------------------------------------------------------------------------------------------

        //Формируем сообщение
        Message4 mess_H;
        mess_H.user_sender = "LOGIN";
        mess_H.mess = "Привет это сообщение всем!";
        
        mess_json.clear();  // Очищаем json перед новым использованием
        // создаем json и преобразуем сообщение
        mess_H.to_json(mess_json);

        //json в строку
        mess_push = mess_json.dump();
        
        
        // Отправляем
        if (send(sockfd, mess_push.c_str(), mess_push.size(), 0) <= 0) {
            cerr << "Отправить не удастся" << endl;
            break;
        }

        // Принимаем ответ
        memset(buffer, 0, BUFFER_SIZE);
        bytes_received = recv(sockfd, buffer, BUFFER_SIZE - 1, 0);
        if (bytes_received <= 0) {
            cerr << "Получение не удалось" << endl;
            break;
        }

        // Полученый ответ передам на логику дальше
        response = buffer;
        handle_incoming_message(response);

        break;
    }

    close(sockfd);
    return 0;
}