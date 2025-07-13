#pragma once
#include "Message.h"
#include "NetworkClient.h"
#include <nlohmann/json.hpp>
#include <string>
#include <memory>

class interaction_chat;

class MessageHandler {
protected:
    std::shared_ptr<NetworkClient> _network;
    std::shared_ptr<interaction_chat> _interaction_chat;
    std::shared_ptr<MessageHandler> _next;
   
public:
    explicit MessageHandler(std::shared_ptr<NetworkClient> network, 
                            std::shared_ptr<interaction_chat> interaction_chat);
    
    virtual ~MessageHandler() = default;
    
    void setNext(std::shared_ptr<MessageHandler> next) {
        _next = next;
    }
    
    virtual bool handle(const std::shared_ptr<Message>& message) = 0;
    
    bool handleNext(const std::shared_ptr<Message>& message);
    
    // Новый метод для установки interaction_chat
    void setInteractionChat(std::shared_ptr<interaction_chat> chat) {
        _interaction_chat = chat;
    }
};


// Обработка для Message50 (Ошибка от сервера)
class HandlerMessage50 : public MessageHandler {
public:
    //using для наследования конструкторов базового класса
    using MessageHandler::MessageHandler;
    
    bool handle(const std::shared_ptr<Message>& message) override;

    void setInteractionChat(std::shared_ptr<interaction_chat> chat) {
        _interaction_chat = chat;
    }
};


// Обработка для Message51 (Передача данных общего чата)
class HandlerMessage51 : public MessageHandler {
public:
    //using для наследования конструкторов базового класса
    using MessageHandler::MessageHandler;
    
    bool handle(const std::shared_ptr<Message>& message) override;
};


// Обработка для Message52 (Передача данных приватного чата)
class HandlerMessage52 : public MessageHandler {
public:
    //using для наследования конструкторов базового класса
    using MessageHandler::MessageHandler;
    
    bool handle(const std::shared_ptr<Message>& message) override;
};


// Обработка для Message53 (Передача списка истории приватных чатов)
class HandlerMessage53 : public MessageHandler {
public:
    //using для наследования конструкторов базового класса
    using MessageHandler::MessageHandler;
    
    bool handle(const std::shared_ptr<Message>& message) override;
};


// Обработка для Message54 (получить список всех юзеров в чате кому написать)
class HandlerMessage54 : public MessageHandler {
public:
    //using для наследования конструкторов базового класса
    using MessageHandler::MessageHandler;
    
    bool handle(const std::shared_ptr<Message>& message) override;
};


// Обработка для Message55 (Ответ сервера логин занят)
class HandlerMessage55 : public MessageHandler {
public:
    //using для наследования конструкторов базового класса
    using MessageHandler::MessageHandler;
    
    bool handle(const std::shared_ptr<Message>& message) override;
};


// Обработка для Message56 (Ответ сервера вернуть имя)
class HandlerMessage56 : public MessageHandler {
public:
    //using для наследования конструкторов базового класса
    using MessageHandler::MessageHandler;
    
    bool handle(const std::shared_ptr<Message>& message) override;
        
    void setInteractionChat(std::shared_ptr<interaction_chat> chat) {
        _interaction_chat = chat;
    }
};