#include "Network.h"
#include "BD.h"
#include "Protocol.h"
#include <nlohmann/json.hpp>
#include <string>
#include <variant>
#include <memory>
#include <iostream>
#include <sys/socket.h>         //для работы с сокетами


std::string online_user_login;

// Обработка входящих сообщений
void handle_incoming_message(const std::string& json_str, int client_sock) {
    try {
        auto msg = parse_message(json_str);
        
        if (!msg) {
            std::cerr << "Ошибка: msg == nullptr" << std::endl;
            return;
        }
        

        // Определяем тип сообщения
        switch(msg->getTupe()) {
            case 1: {
                // Приводим к конкретному типу
                auto* m1 = dynamic_cast<Message1*>(msg.get());
                std::cout << "Запрос входа в систему:" << m1->login << std::endl;
                std::cout << "Запрос входа в систему:" << m1->pass << std::endl;
                online_user_login = m1->login;
                
                // Отправляем ответ
                Message50 mess_class;
                mess_class.status_request = true;
                json mess_json;
                mess_class.to_json(mess_json);
                std::string mess_push = mess_json.dump();
    
                send(client_sock, mess_push.c_str(), mess_push.size(), 0);
                // Логика обработки
                break;
            }
            case 2: {
                auto* m2 = dynamic_cast<Message2*>(msg.get());
                if (!m2) {
                    std::cerr << "Ошибка приведения типа для Message2" << std::endl;
                    break;
                }
                std::cout << "Получил логин:" << m2->login << std::endl;
                std::cout << "Получил пароль:" << m2->pass << std::endl;
                std::cout << "Получил имя:" << m2->name << std::endl;
                std::shared_ptr<User> user = std::make_shared<User>(m2->login, m2->pass, m2->name);
                BD_write_User(user);
                online_user_login = m2->login;
                
                // Отправляем ответ
                Message50 mess_class;
                mess_class.status_request = true;
                json mess_json;
                mess_class.to_json(mess_json);
                std::string mess_push = mess_json.dump();
    
                send(client_sock, mess_push.c_str(), mess_push.size(), 0);
                // Логика обработки
                break;
            }
            case 3: {
                auto* m3 = dynamic_cast<Message3*>(msg.get());
                std::cout << "Получил отправителя:" << m3->user_sender << std::endl;
                std::cout << "Получил полцчателя:" << m3->user_recipient << std::endl;
                std::cout << "Получил сообщение:" << m3->mess << std::endl;
                std::shared_ptr<User> user_sender = BD_search_User(m3->user_sender);
                std::shared_ptr<User> user_recipient = BD_search_User(m3->user_recipient);
                
                if (user_sender == nullptr || user_recipient == nullptr)
                {
                    std::cout << "Ошибка такого юзера не существует";
                    break;
                }

                if(user_sender->getLogin() != online_user_login){
                    std::cout << "Конец сессии, авторизируйтесь заного";
                }
                write_Chat_P(user_sender, user_recipient, m3->mess);
                std::vector<std::pair<std::string, std::string>> history_chat_P; 
                load_Chat_P(user_sender, user_recipient, history_chat_P);
                std::cout << "Отправка сообщения 52";
                // Отправляем ответ
                Message52 mess_class;
                mess_class.history_chat_P = history_chat_P;
                json mess_json;
                mess_class.to_json(mess_json);
                std::string mess_push = mess_json.dump();
    
                send(client_sock, mess_push.c_str(), mess_push.size(), 0);
                
                
                // Логика обработки
                break;
            }
            case 4: {
                auto* m4 = dynamic_cast<Message4*>(msg.get());
                std::cout << "Получил отправителя:" << m4->user_sender << std::endl;
                std::cout << "Получил сообщение:" << m4->mess << std::endl;
                std::shared_ptr<User> user_sender = BD_search_User(m4->user_sender);
                
                if (user_sender == nullptr)
                {
                    std::cout << "Ошибка такого юзера не существует";
                    break;
                }

                if(user_sender->getLogin() != online_user_login){
                    std::cout << "Конец сессии, авторизируйтесь заного";
                }

                write_Chat_H(user_sender, m4->mess);
                std::vector<std::pair<std::string, std::string>> history_chat_H; 
                load_Chat_H(history_chat_H);
                // Отправляем ответ
                Message51 mess_class;
                mess_class.history_chat_H = history_chat_H;
                json mess_json;
                mess_class.to_json(mess_json);
                std::string mess_push = mess_json.dump();
    
                send(client_sock, mess_push.c_str(), mess_push.size(), 0);
                
                // Логика обработки
                std::cout << "Сообщение прошло";
                break;
            }

            // остальное потом
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}