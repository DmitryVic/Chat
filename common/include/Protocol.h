#pragma once
#include <nlohmann/json.hpp>
#include <string>
#include <variant>
#include <memory>

using json = nlohmann::json;

// Абстрактный базовый класс
class Message {
public:
    virtual int getTupe() const = 0; 
    virtual void to_json(json& j) const = 0;
    virtual void from_json(const json& j) = 0;
    virtual ~Message() = default;

    //Создание сообщений
    static std::unique_ptr<Message> create(int type);
};


// Передавча логина и пароля
class Message1 : public Message {
public:
    std::string login;
    std::string pass;
    
    int getTupe() const override { return 1; }
    
    void to_json(json& j) const override;
    
    void from_json(const json& j) override;
};

// Регистрация
class Message2 : public Message {
public:
    std::string login;
    std::string pass;
    std::string name;
    
    int getTupe() const override { return 2; }
    
    void to_json(json& j) const override;
    
    void from_json(const json& j) override;
};

// Ответ сервера на запрос
class Message3 : public Message {
public:
    bool status_request;
    
    int getTupe() const override { return 3; }
    
    void to_json(json& j) const override;
    
    void from_json(const json& j) override;
};


std::unique_ptr<Message> parse_message(const std::string& json_str);


