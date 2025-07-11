#include "NetworkServer.h"
#include "BD.h"
#include "Message.h"
#include <nlohmann/json.hpp>
#include <string>
#include <variant>
#include <memory>
#include <iostream>
#include <sys/socket.h>         //для работы с сокетами

// для логирования, авториз и рег сохраняют, остальные методы проверяют на соответствие
std::string online_user_login = "";

//ответ bool об ошибке или успешности операции
void srandart_answer(bool status, int client_sock){
    Message50 mess_class;
    mess_class.status_request = status;
    json mess_json;
    mess_class.to_json(mess_json);
    std::string mess_push = mess_json.dump();
    send(client_sock, mess_push.c_str(), mess_push.size(), 0);
}

// Обработка входящих сообщений
void handle_incoming_message(const std::string& json_str, int client_sock) {
    try {
        auto msg = parse_message(json_str);
        
        if (!msg) {
            std::cerr << "Ошибка: msg == nullptr" << std::endl;
            // Отправляем ответ об ошибке
            srandart_answer(false, client_sock);
            return;
        }
        

        // Определяем тип сообщения
        switch(msg->getTupe()) {
            case 1: {
                // Приводим к shared_ptr
                auto m1 = std::dynamic_pointer_cast<Message1>(msg);

                std::shared_ptr<User> user = BD_search_User(m1->login);
                // есть ли такой пользователь в БД и совпадает ли логин, пароль
                if (user && (m1->login == user->getLogin() && m1->pass == user->getPass()))
                {
                    online_user_login = m1->login;
                    // Отправляем ответ
                    srandart_answer(true, client_sock);
                    break;
                }
                std::cerr << "Error: Не верный логин или пароль" << std::endl;
                srandart_answer(false, client_sock);
                break;
            }
            case 2: {
                auto m2 = std::dynamic_pointer_cast<Message2>(msg);

                if(m2->name.empty() || m2->login.empty() || m2->pass.empty()){
                    std::cerr << "Error: Не верные данные для регистрации (сообщение 2)";
                    // Отправляем ответ об ошибке
                    srandart_answer(false, client_sock);
                    break;
                }

                if (BD_search_User(m2->login))
                {
                    std::cerr << "Error: Ошибка регистрации - логин занят";
                    // Отправляем ответ об ошибке
                    Message55 mess_class;
                    mess_class.status_request = false;
                    json mess_json;
                    mess_class.to_json(mess_json);
                    std::string mess_push = mess_json.dump();
        
                    send(client_sock, mess_push.c_str(), mess_push.size(), 0);
                    break;
                }
                

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
                // Передача данных приватного чата. получение сообщения в приватного чат
                auto m3 = std::dynamic_pointer_cast<Message3>(msg);
                std::shared_ptr<User> user_sender = BD_search_User(m3->user_sender);
                std::shared_ptr<User> user_recipient = BD_search_User(m3->user_recipient);
                
                if (user_sender == nullptr || user_recipient == nullptr)
                {
                    std::cerr << "Error: Не верные данные авторизации авторизованного юзера (сообщение 3)";
                    // Отправляем ответ об ошибке
                    srandart_answer(false, client_sock);
                    break;
                }

                if(user_sender->getLogin() != online_user_login){
                    std::cerr << "Error: Попытка получить ответ не авторизованного юзера (сообщение 3)";
                    // Отправляем ответ об ошибке
                    srandart_answer(false, client_sock);
                    break;
                }

                write_Chat_P(user_sender, user_recipient, m3->mess);
                std::vector<std::pair<std::string, std::string>> history_chat_P; 
                load_Chat_P(user_sender, user_recipient, history_chat_P);

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
                auto m4 = std::dynamic_pointer_cast<Message4>(msg);
                std::shared_ptr<User> user_sender = BD_search_User(m4->user_sender);
                
                if (user_sender == nullptr)
                {
                    std::cerr << "Error: Не верные данные авторизации авторизованного юзера (сообщение 4)";
                    // Отправляем ответ об ошибке
                    srandart_answer(false, client_sock);
                    break;
                }

                if(user_sender->getLogin() != online_user_login){
                    std::cerr << "Error: Попытка получить ответ не авторизованного юзера (сообщение 4)";
                    // Отправляем ответ об ошибке
                    srandart_answer(false, client_sock);
                    break;
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
                break;
            }
            case 5: {
                // Запрос на получение списка приватных чатов
                
                auto m5 = std::dynamic_pointer_cast<Message5>(msg);
                //получаем пользователя и проверяем на nullptr
                std::shared_ptr<User> user_sender = BD_search_User(m5->my_login);
                
                //есть ли пользователь в базе и логин залогированного пользователя?
                if (user_sender && online_user_login == m5->my_login)
                {
                    // получаем данные с БД
                    std::vector<std::pair<std::string, std::string>> list_Chat_P = my_chat_P(m5->my_login);
                    
                    // БД - нет данных
                    if (list_Chat_P.empty())
                    {
                        // Отправляем ответ об ошибке
                        srandart_answer(false, client_sock);
                        break;
                    }
                    
                    // Отправляем ответ
                    Message53 mess_class;
                    mess_class.list_chat_P = list_Chat_P;
                    json mess_json;
                    mess_class.to_json(mess_json);
                    std::string mess_push = mess_json.dump();
        
                    send(client_sock, mess_push.c_str(), mess_push.size(), 0);
                    break;
                }
                
                // Отправляем ответ об ошибке
                srandart_answer(false, client_sock);
                break;
            }
            case 6: {
                // Запрос на получение списока всех юзеров в чате кому написать
                
                auto m6 = std::dynamic_pointer_cast<Message6>(msg);
                //получаем пользователя и проверяем на nullptr
                std::shared_ptr<User> user_sender = BD_search_User(m6->my_login);
                if (!user_sender)
                {
                    // Отправляем ответ об ошибке
                    srandart_answer(false, client_sock);
                    break;
                }
                
                //есть ли пользователь в базе и логин залогированного пользователя?
                if (user_sender && online_user_login == m6->my_login)
                {
                    // получаем данные с БД
                    std::vector<std::pair<std::string, std::string>> list_Users = list_all_User(m6->my_login);
                    
                    // БД - нет данных
                    if (list_Users.empty())
                    {
                        // Отправляем ответ об ошибке
                        srandart_answer(false, client_sock);
                        break;
                    }
                    
                    // Отправляем ответ
                    Message54 mess_class;
                    mess_class.list_Users = list_Users;
                    json mess_json;
                    mess_class.to_json(mess_json);
                    std::string mess_push = mess_json.dump();
        
                    send(client_sock, mess_push.c_str(), mess_push.size(), 0);
                    break;
                }
                
                // Отправляем ответ об ошибке
                srandart_answer(false, client_sock);
                break;
            }
            case 7: {
                // Запрос юзера получить свое имя
                
                auto m7 = std::dynamic_pointer_cast<Message7>(msg);
                //получаем пользователя и проверяем на nullptr
                std::shared_ptr<User> user = BD_search_User(m7->my_login);

                //есть ли пользователь в базе и логин залогированного пользователя?
                if (user && online_user_login == m7->my_login)
                {
                    // Отправляем ответ
                    Message56 mess_class;
                    mess_class.my_name = user->getName();
                    json mess_json;
                    mess_class.to_json(mess_json);
                    std::string mess_push = mess_json.dump();
        
                    send(client_sock, mess_push.c_str(), mess_push.size(), 0);
                    break;
                }
                
                // Отправляем ответ об ошибке
                srandart_answer(false, client_sock);
                break;
            }
            default:
                std::cerr << "Error handle_incoming_message server: Не верный «Тип» в сообщении: "  << msg->getTupe() << std::endl;
                // Отправляем ответ об ошибке
                srandart_answer(false, client_sock);
            break;
            // остальное потом
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        // Отправляем ответ об ошибке
        srandart_answer(false, client_sock);
    }
}