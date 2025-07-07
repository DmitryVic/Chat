#include "Network.h"
#include "BD.h"
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
                std::cout << "Поиск не сущ логина без файла:" << std::endl;
                std::shared_ptr<User> user0 = BD_search_User("ldjd");
                if (user0 != nullptr)
                {
                    std::cout << "Найден Пользователь ldjd" << std::endl;
                }
                else{
                    std::cout << "НЕ Найден Пользователь ldjd" << std::endl;
                }

                auto* m2 = dynamic_cast<Message2*>(msg.get());
                std::cout << "Получил логин:" << m2->login << std::endl;
                std::cout << "Получил пароль:" << m2->pass << std::endl;
                std::cout << "Получил имя:" << m2->name << std::endl;
                std::shared_ptr<User> user = std::make_shared<User>(m2->login, m2->pass, m2->name);
                BD_write_User(user);
                std::shared_ptr<User> user2 = BD_search_User(user->getLogin());
                if (user2 != nullptr)
                {
                    std::cout << "Найден Пользователь" << std::endl
                    << user2->getLogin() << std::endl
                    << user2->getPass() << std::endl
                    << user2->getName() << std::endl;
                }
                else{
                    std::cout << "Не смог считать пользователя" << std::endl;
                }
                
                std::cout << "Поиск не сущ логина без файла:" << std::endl;
                std::shared_ptr<User> user4 = BD_search_User("44444");
                if (user4 != nullptr)
                {
                    std::cout << "Найден Пользователь 44444" << std::endl;
                }
                else{
                    std::cout << "НЕ Найден Пользователь 44444" << std::endl;
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