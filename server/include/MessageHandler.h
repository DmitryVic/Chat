#pragma once
#include "NetworkServer.h"
#include "BD.h"
#include "Message.h"
#include <nlohmann/json.hpp>
#include <string>
#include <variant>
#include <memory>

// Базовый класс обработчика - метод цепочки ответственности
class MessageHandler {
protected:
    std::shared_ptr<DataBase> _db;
    std::shared_ptr<NetworkServer> _network;
    //Паттерн Цепочка ответственности каждый будет пытаться обработать, но сможет только 1
    std::unique_ptr<MessageHandler> _next;
    //Для авторизации пользователя
    std::string online_user_login;

    
public:
    MessageHandler(std::shared_ptr<DataBase> db, 
                   std::shared_ptr<NetworkServer> network)
        : _db(db), _network(network), _next(nullptr) {}
    
    virtual ~MessageHandler() = default;
    
    void setNext(std::unique_ptr<MessageHandler> next) {
        _next = std::move(next);
    }
    
    virtual bool handle(const std::shared_ptr<Message>& message) = 0;
    
    bool handleNext(const std::shared_ptr<Message>& message) {
        if (_next) return _next->handle(message);
        return false;
    }
};

// Обработка для Message1 (авторицация)
class HandlerMessage1 : public MessageHandler {
public:
    //using для наследования конструкторов базового класса
    using MessageHandler::MessageHandler;
    
    bool handle(const std::shared_ptr<Message>& message) override {
        // Проверяем, наше ли это сообщение
        if (message->getTupe() != 1) {
            // Не наше - передаем следующему в цепочке
            return handleNext(message);
        }
        //обрабатываем
        auto m1 = std::dynamic_pointer_cast<Message1>(message);
        
        // Логика обработки
        std::shared_ptr<User> user = _db->search_User(m1->login);
        bool authSuccess = user && (m1->pass == user->getPass());
        
        //Фиксация авторизации
        online_user_login = m1->login;

        // Формируем ответ
        Message50 response;
        response.status_request = authSuccess;
        
        // Отправляем ответ через сеть
        json j;
        response.to_json(j);
        _network->sendMess(j.dump());
        
        return true;  // Сообщение обработано
    }
};


// Обработчик для Message2 (регистрация)
class HandlerMessage2 : public MessageHandler {
public:
    using MessageHandler::MessageHandler;
    
    bool handle(const std::shared_ptr<Message>& message) override {
        if (message->getTupe() != 2) {
            return handleNext(message);
        }
        
        auto m2 = std::dynamic_pointer_cast<Message2>(message);
        
        if (m2->name.empty() || m2->login.empty() || m2->pass.empty()) {
            Message50 response;
            response.status_request = false;
            json j;
            response.to_json(j);
            _network->sendMess(j.dump());
            return true;
        }
        
        if (_db->search_User(m2->login)) {
            Message55 response;
            response.status_request = false;
            json j;
            response.to_json(j);
            _network->sendMess(j.dump());
            return true;
        }
        
        std::shared_ptr<User> user = std::make_shared<User>(m2->login, m2->pass, m2->name);
        _db->write_User(user);
        
        Message50 response;
        response.status_request = true;
        json j;
        response.to_json(j);
        _network->sendMess(j.dump());
        
        return true;
    }
};


// Обработчик для Message3 (Передача данных приватного чата)
class HandlerMessage3 : public MessageHandler {
public:
    using MessageHandler::MessageHandler;
    
    bool handle(const std::shared_ptr<Message>& message) override {
        if (message->getTupe() != 3) {
            return handleNext(message);
        }
        
        auto m3 = std::dynamic_pointer_cast<Message3>(message);
        std::shared_ptr<User> user_sender = _db->search_User(m3->user_sender);
        std::shared_ptr<User> user_recipient = _db->search_User(m3->user_recipient);
        
        if (user_sender == nullptr || user_recipient == nullptr)
        {
            //Error: Не верные данные авторизации авторизованного юзера (сообщение 3)
            // Отправляем ответ об ошибке
            Message50 response;
            response.status_request = false;
            json j;
            response.to_json(j);
            _network->sendMess(j.dump());
            return true;
        }

        if(user_sender->getLogin() != online_user_login){
            //Error: Попытка получить ответ не авторизованного юзера (сообщение 3)"
            // Отправляем ответ об ошибке
            Message50 response;
            response.status_request = false;
            json j;
            response.to_json(j);
            _network->sendMess(j.dump());
            return true;
        }

        _db->write_Chat_P(user_sender, user_recipient, m3->mess);
        std::vector<std::pair<std::string, std::string>> history_chat_P; 
        _db->load_Chat_P(user_sender, user_recipient, history_chat_P);

        // Отправляем ответ
        Message52 mess_class;
        mess_class.history_chat_P = history_chat_P;
        json mess_json;
        json j;
        mess_class.to_json(j);
        _network->sendMess(j.dump());
        
        return true;
    }
};


// Обработчик для неизвестных сообщений
class HandlerErr : public MessageHandler {
public:
    using MessageHandler::MessageHandler;
    
    bool handle(const std::shared_ptr<Message>& message) override {
        std::cerr << "Неизвестный тип сообщения: " << message->getTupe() << std::endl;
        
        Message50 response;
        response.status_request = false;
        
        json j;
        response.to_json(j);
        _network->sendMess(j.dump());
        
        return true;
    }
};