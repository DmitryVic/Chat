#include "Protocol.h"
#include <nlohmann/json.hpp>
#include <string>
#include <variant>
#include <memory>
#include <iostream>


/*=====================================
        СООБЩЕНИЯ ОТ КЛИЕНТА
=====================================*/


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


/*=====================================
        СООБЩЕНИЯ ОТ СЕРВЕРА
=====================================*/


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

// Ответ сервера на запрос true or false
void Message50::to_json(json& j) const{
    j = {{"type", 50}, {"status_request", status_request}};
}

// Ответ сервера на запрос true or false
void Message50::from_json(const json& j){
    status_request = j.at("status_request").get<bool>();
}


// Передача данных общего чата
void Message51::to_json(json& j) const{
    j = {{"type", 51}, {"history_chat_H", history_chat_H}};
}

// Передача данных общего чата
void Message51::from_json(const json& j){
    history_chat_H = j.at("history_chat_H").get<std::vector<std::pair<std::string, std::string>>>();
}


// Передача данных приватного чата
void Message52::to_json(json& j) const{
    j = {{"type", 52}, {"history_chat_P", history_chat_P}};
}

// Передача данных приватного чата
void Message52::from_json(const json& j){
    history_chat_P = j.at("history_chat_P").get<std::vector<std::pair<std::string, std::string>>>();
}

std::unique_ptr<Message> Message::create(int type) {
    switch(type) {
        case 1: return std::make_unique<Message1>();
        case 2: return std::make_unique<Message2>();
        case 3: return std::make_unique<Message3>();
        case 4: return std::make_unique<Message4>();
        case 50: return std::make_unique<Message50>();
        case 51: return std::make_unique<Message51>();
        case 52: return std::make_unique<Message52>();
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


