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
                auto* m50 = dynamic_cast<Message50*>(msg.get());
                std::string bool_w; 
                (m50->status_request) ? bool_w = "trye" : bool_w = "false";
                std::cerr << "Ответ статуса:" << bool_w << std::endl;
                // Логика обработки
                break;
            }
            case 51: {
                auto* m51 = dynamic_cast<Message51*>(msg.get());
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
                auto* m52 = dynamic_cast<Message52*>(msg.get());
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
            // остальное потом
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}