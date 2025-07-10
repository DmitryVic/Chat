#include "Network.h"
#include "Protocol.h"
#include <nlohmann/json.hpp>
#include <string>
#include <variant>
#include <memory>
#include <iostream>

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
