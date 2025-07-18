#include "Message.h"
#include <nlohmann/json.hpp>
#include <string>
#include <variant>
#include <memory>
#include <utility>
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
        j = {{"type", 4}, {"login_user_sender", login_user_sender}, {"name_user_sender", name_user_sender}, {"mess", mess}};
    }

// Отправка сообщения в общий чат 
void Message4::from_json(const json& j){
    login_user_sender = j.at("login_user_sender").get<std::string>();
    name_user_sender = j.at("name_user_sender").get<std::string>();
    mess = j.at("mess").get<std::string>();
}

// Запрос на получение списка приватных чатов
void Message5::to_json(json& j) const{
    j = {{"type", 5}, {"my_login", my_login}};
}

// Запрос на получение списка приватных чатов
void Message5::from_json(const json& j){
    my_login = j.at("my_login").get<std::string>();
}

// Запрос на получение списока всех юзеров в чате кому написать
void Message6::to_json(json& j) const{
    j = {{"type", 6}, {"my_login", my_login}};
}

// Запрос на получение списока всех юзеров в чате кому написать
void Message6::from_json(const json& j){
    my_login = j.at("my_login").get<std::string>();
}

// Запрос юзера получить свое имя
void Message7::to_json(json& j) const{
    j = {{"type", 7}, {"my_login", my_login}};
}

// Запрос юзера получить свое имя
void Message7::from_json(const json& j){
    my_login = j.at("my_login").get<std::string>();
}


// обновить данные приватного чата
void Message8::to_json(json& j) const{
    j = {{"type", 8}, {"user_sender", user_sender}, {"user_recipient", user_recipient}};
}

// обновить данные приватного чата
void Message8::from_json(const json& j){
    user_sender = j.at("user_sender").get<std::string>();
    user_recipient = j.at("user_recipient").get<std::string>();
}


// обновить данные общего чата
void Message9::to_json(json& j) const{
    j = {{"type", 9}, {"user_sender", user_sender}};
}

// обновить данные общего чата
void Message9::from_json(const json& j){
    user_sender = j.at("user_sender").get<std::string>();
}


/*=====================================
        СООБЩЕНИЯ ОТ СЕРВЕРА
=====================================*/


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
    history_chat_H = j.at("history_chat_H").get<std::vector<std::vector<std::string>>>();
}


// Передача данных приватного чата
void Message52::to_json(json& j) const{
    j = {{"type", 52}, {"history_chat_P", history_chat_P}, {"login_name_friend", login_name_friend}, {"login_name_MY", login_name_MY}};
}

// Передача данных приватного чата
void Message52::from_json(const json& j){
    history_chat_P = j.at("history_chat_P").get<std::vector<std::pair<std::string, std::string>>>();
    login_name_friend = j.at("login_name_friend").get<std::pair<std::string, std::string>>(); 
    login_name_MY = j.at("login_name_MY").get<std::pair<std::string, std::string>>(); 
}


// Передача списка истории приватных чатов
void Message53::to_json(json& j) const{
    j = {{"type", 53}, {"list_chat_P", list_chat_P}};
}

// Передача списка истории приватных чатов
void Message53::from_json(const json& j){
    list_chat_P = j.at("list_chat_P").get<std::vector<std::pair<std::string, std::string>>>();
}


// получить список всех юзеров в чате кому написать
void Message54::to_json(json& j) const{
    j = {{"type", 54}, {"list_Users", list_Users}};
}

// получить список всех юзеров в чате кому написать
void Message54::from_json(const json& j){
    list_Users = j.at("list_Users").get<std::vector<std::pair<std::string, std::string>>>();
}


// Ответ сервера логин занят
void Message55::to_json(json& j) const{
    j = {{"type", 55}, {"status_request", status_request}};
}

// Ответ сервера логин занят
void Message55::from_json(const json& j){
    status_request = j.at("status_request").get<bool>();
}

// получение имени юзера
void Message56::to_json(json& j) const{
    j = {{"type", 56}, {"my_name", my_name}};
}

// получение имени юзера
void Message56::from_json(const json& j){
    my_name = j.at("my_name").get<std::string>();
}


std::unique_ptr<Message> Message::create(int type) {
    switch(type) {
        case 1: return std::make_unique<Message1>();
        case 2: return std::make_unique<Message2>();
        case 3: return std::make_unique<Message3>();
        case 4: return std::make_unique<Message4>();
        case 5: return std::make_unique<Message5>();
        case 6: return std::make_unique<Message6>();
        case 7: return std::make_unique<Message7>();
        case 8:  return std::make_unique<Message8>();
        case 9: return std::make_unique<Message9>();
        case 50: return std::make_unique<Message50>();
        case 51: return std::make_unique<Message51>();
        case 52: return std::make_unique<Message52>();
        case 53: return std::make_unique<Message53>();
        case 54: return std::make_unique<Message54>();
        case 55: return std::make_unique<Message55>();
        case 56: return std::make_unique<Message56>();
        default: throw std::runtime_error("Неизвестный тип сообщения: " + std::to_string(type));
    }
}


std::shared_ptr<Message> parse_message(const std::string& json_str) {
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
        
    }
    catch (const json::exception& e) {
        std::cerr << "Ошибка протокола обмена 1: исключение JSON в parse_message " << std::endl;
        return {};
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка протокола обмена 2: Не стандартное исключение (parse_message JSON)" << std::endl;
        return {};
    } 
}


