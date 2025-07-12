#include "Message.h"
#include "NetworkClient.h"
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




// Обработка входящих сообщений
void handle_incoming_message(const std::string& json_str) {
    try {
        auto msg = parse_message(json_str);
        
        // Определяем тип сообщения
        switch(msg->getTupe()) {
            case 50: {
                auto m50 = std::dynamic_pointer_cast<Message50>(msg);
                std::string bool_w; 
                (m50->status_request) ? bool_w = "trye" : bool_w = "false";
                std::cerr << "Ответ статуса:" << bool_w << std::endl;
                // Логика обработки
                break;
            }
            case 51: {
                auto m51 = std::dynamic_pointer_cast<Message51>(msg);
                try
                {
                    std::vector<std::pair<std::string, std::string>> history_chat_H = m51->history_chat_H;
                    for(auto a: history_chat_H){
                        std::cerr << a.first << "      " << a.second << "\n";
                    }
                }
                catch(const std::exception& e)
                {
                    std::cerr << e.what() << '\n';
                }
                // Логика обработки
                break;
            }
            case 52: {
                auto m52 = std::dynamic_pointer_cast<Message52>(msg);
                try
                {
                    std::vector<std::pair<std::string, std::string>> history_chat_P = m52->history_chat_P;
                    for(auto a: history_chat_P){
                        std::cerr << a.first << "      " << a.second << "\n";
                    }
                }
                catch(const std::exception& e)
                {
                    std::cerr << e.what() << '\n';
                }
                // Логика обработки
                break;
            }
            case 53: {
                // Получение списка истории приватных чатов
                auto m53 = std::dynamic_pointer_cast<Message53>(msg);
                try
                {
                    std::vector<std::pair<std::string, std::string>> list_Chat_P = m53->list_chat_P;
                    std::cerr << "Получение списка истории приватных чатов: \n";
                    for(auto a: list_Chat_P){
                        std::cerr << "Login: " << a.first << "   Name: " << a.second << "\n";
                    }
                }
                catch(const std::exception& e)
                {
                    std::cerr << e.what() << '\n';
                }
                break;
            }
            case 54: {
                // получить список всех юзеров в чате кому написать
                auto m54 = std::dynamic_pointer_cast<Message54>(msg);
                try
                {
                    std::vector<std::pair<std::string, std::string>> list_Users = m54->list_Users;
                    std::cerr << "Получить список всех юзеров в чате кому написать: \n";
                    for(auto a: list_Users){
                        std::cerr << "Login: " << a.first << "   Name: " << a.second << "\n";
                    }
                }
                catch(const std::exception& e)
                {
                    std::cerr << e.what() << '\n';
                }
                break;
            }
            case 55: {
                auto m55 = std::dynamic_pointer_cast<Message55>(msg);
                std::string bool_w; 
                (m55->status_request) ? bool_w = "trye" : bool_w = "false";
                std::cerr << "Логин занят:" << bool_w << std::endl;
                // Логика обработки
                break;
            }
            case 56: {
                auto m56 = std::dynamic_pointer_cast<Message56>(msg);
                
                std::cerr << m56->my_name << ", добро пожаловать в чат!" << std::endl;
                break;
            }
            // остальное потом
            default:
                std::cerr << "Error handle_incoming_message client: Не верный «Тип» в сообщении" << std::endl;
                //реализовать возврат в главное меню
            break;
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}






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
        Message1 mess_class;
        mess_class.login = "LOGIN";
        mess_class.pass = "пароль";

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
        Message3 mess_H;
        mess_H.user_sender = "LOGIN";
        mess_H.user_recipient = "Log";
        mess_H.mess = "Сообщение для теста";

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