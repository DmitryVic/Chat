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
            case 1: {
                // Приводим к конкретному типу
                auto* m1 = dynamic_cast<Message1*>(msg.get());
                std::cout << "Запрос входа в систему:" << m1->login << std::endl;
                std::cout << "Запрос входа в систему:" << m1->pass << std::endl;
                // Логика обработки
                break;
            }
            case 2: {
                auto* m2 = dynamic_cast<Message2*>(msg.get());
                std::cout << "Регистрация:" << m2->name << std::endl;
                // Логика обработки
                break;
            }

            // остальное потом
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}