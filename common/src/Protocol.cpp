#include "Protocol.h"
#include <nlohmann/json.hpp>
#include <string>
#include <variant>
#include <memory>
#include <iostream>


// Передавча логина и пароля
void Message1::to_json(json& j) const{
    j = {{"type", 1}, {"login", login}, {"pass", pass}};
}

// Передавча логина и пароля
void Message1::from_json(const json& j) {
    login = j.at("login").get<std::string>();
    pass = j.at("pass").get<std::string>();
}


void Message2::to_json(json& j) const{
        j = {{"type", 2}, {"login", login}, {"pass", pass}, {"name", name}};
    }
    
void Message2::from_json(const json& j){
    login = j.at("login").get<std::string>();
    pass = j.at("pass").get<std::string>();
    name = j.at("name").get<std::string>();
}


//-------------
// Отправка сообщения в приватный чат
void Message3::to_json(json& j) const{
    j = {{"type", 3}, {"user_sender", user_sender}, {"user_recipient", user_recipient}, {"mess", mess}};
}


// Отправка сообщения в приватный чат
void Message3::from_json(const json& j) {
    user_sender = j.at("user_sender").get<std::string>();
    user_recipient = j.at("user_recipient").get<std::string>();
    mess = j.at("mess").get<std::string>();
}


// Отправка сообщения в общий чат
void Message4::to_json(json& j) const{
        j = {{"type", 4}, {"user_sender", user_sender}, {"mess", mess}};
    }

// Отправка сообщения в общий чат 
void Message4::from_json(const json& j){
    user_sender = j.at("user_sender").get<std::string>();
    mess = j.at("mess").get<std::string>();
}
//-------------

void Message50::to_json(json& j) const{
    j = {{"type", 50}, {"status_request", status_request}};
}

void Message50::from_json(const json& j){
    status_request = j.at("status_request").get<bool>();
}


std::unique_ptr<Message> Message::create(int type) {
    switch(type) {
        case 1: return std::make_unique<Message1>();
        case 2: return std::make_unique<Message2>();
        case 3: return std::make_unique<Message3>();
        case 4: return std::make_unique<Message4>();
        case 50: return std::make_unique<Message50>();
        default: throw std::runtime_error("Неизвестный тип сообщения: " + std::to_string(type));
    }
}


std::unique_ptr<Message> parse_message(const std::string& json_str) {
    try {
        json j = json::parse(json_str);
        
        // Определяем тип сообщения
        if (!j.contains("type")) {
            throw std::runtime_error("Отсутствует поле «Тип» в сообщении");
        }
        int type = j["type"].get<int>();
        
        // Создаем объект нужного типа
        auto msg = Message::create(type);

        // Заполняем его данными из JSON
        msg->from_json(j);
        
        return msg;
        
    } catch (const json::exception& e) {
        throw std::runtime_error("Ошибка JSON: " + std::string(e.what()));
    }
}


