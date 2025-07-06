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
            case 3: {
                auto* m3 = dynamic_cast<Message3*>(msg.get());
                std::cout << "Ответ статуса:" << m3->status_request << std::endl;
                // Логика обработки
                break;
            }
            // остальное потом
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}