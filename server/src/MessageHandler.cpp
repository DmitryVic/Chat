#include "MessageHandler.h"
#include "NetworkServer.h"
#include "BD.h"
#include "Message.h"
#include <nlohmann/json.hpp>
#include "hashPass.h"
#include <string>
#include <variant>
#include <memory>
#include "Core.h"
#include <utility>


bool MessageHandler::handleNext(const std::shared_ptr<Message>& message) {
    if (_next) return _next->handle(message);
    return false;
}


// Обработка для Message1 (авторицация)
bool HandlerMessage1::handle(const std::shared_ptr<Message>& message) {
    // Проверяем, наше ли это сообщение
    if (message->getTupe() != 1) {
        // Не наше - передаем следующему в цепочке
        return handleNext(message);
    }
    //обрабатываем
    auto m1 = std::dynamic_pointer_cast<Message1>(message);
    
    // Логика обработки
    std::shared_ptr<User> user = _db->search_User(m1->login);

    bool authSuccess = user && (hashPassword(m1->pass) == user->getPass());
    //Фиксация авторизации
    if (authSuccess)
        online_user_login = user->getLogin();
    

    // Формируем ответ
    Message50 response;
    response.status_request = authSuccess;
    
    // Отправляем ответ через сеть
    json j;
    response.to_json(j);
    _network->sendMess(j.dump());
    
    return true;  // Сообщение обработано
}

// Обработчик для Message2 (регистрация)
bool HandlerMessage2::handle(const std::shared_ptr<Message>& message) {
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

    

    std::shared_ptr<User> user = std::make_shared<User>(m2->login, hashPassword(m2->pass), m2->name);
    _db->write_User(user);
    //Фиксация авторизации
    online_user_login = user->getLogin();

    Message50 response;
    response.status_request = true;
    json j;
    response.to_json(j);
    _network->sendMess(j.dump());
    
    return true;
}



// Обработчик для Message3 (Передача данных приватного чата)
bool HandlerMessage3::handle(const std::shared_ptr<Message>& message){
    if (message->getTupe() != 3) {
        return handleNext(message);
    }
    
    auto m3 = std::dynamic_pointer_cast<Message3>(message);
    std::shared_ptr<User> user_sender = _db->search_User(m3->user_sender);
    std::shared_ptr<User> user_recipient = _db->search_User(m3->user_recipient);
    
    if (user_sender == nullptr || user_recipient == nullptr)
    {
        std::cerr << "ошибка бд: "  << std::endl;
        //Error: Не верные данные авторизации авторизованного юзера (сообщение 3)
        // Отправляем ответ об ошибке
        Message50 response;
        response.status_request = false;
        json j;
        response.to_json(j);
        _network->sendMess(j.dump());
        throw std::runtime_error("HandlerMessage3: Закрываю соединение...");
    }

    if(online_user_login !=  user_sender->getLogin()){
        std::cerr << "Пользователь присылает не верные данные или он не авторизован"  << std::endl;
        //Error: Попытка получить ответ не авторизованного юзера (сообщение 3)"
        // Отправляем ответ об ошибке
        Message50 response;
        response.status_request = false;
        json j;
        response.to_json(j);
        _network->sendMess(j.dump());
        throw std::runtime_error("HandlerMessage3: Закрываю соединение...");
    }

    _db->write_Chat_P(user_sender, user_recipient, m3->mess);
    std::vector<std::pair<std::string, std::string>> history_chat_P; 
    _db->load_Chat_P(user_sender, user_recipient, history_chat_P);

    // Отправляем ответ
    Message52 mess_class;
    mess_class.history_chat_P = history_chat_P;
    mess_class.login_name_MY.first = user_sender->getLogin();
    mess_class.login_name_MY.second = user_sender->getName();
    mess_class.login_name_friend.first = user_recipient->getLogin();
    mess_class.login_name_friend.second = user_recipient->getName();
    json mess_json;
    json j;
    mess_class.to_json(j);
    _network->sendMess(j.dump());
    
    return true;
}


// Обработчик для Message4 (Передача данных общего чата)
bool HandlerMessage4::handle(const std::shared_ptr<Message>& message){
    
    if (message->getTupe() != 4) {
        return handleNext(message);
    }

    auto m4 = std::dynamic_pointer_cast<Message4>(message);
    std::shared_ptr<User> user_sender = _db->search_User(m4->login_user_sender);
    
    if (user_sender == nullptr)
    {
        std::cerr << "Error: Не верные данные авторизации авторизованного юзера (сообщение 4)";
        // Отправляем ответ об ошибке
        Message50 response;
        response.status_request = false;
        json j;
        response.to_json(j);
        _network->sendMess(j.dump());
        throw std::runtime_error("HandlerMessage4: Закрываю соединение...");
    }

    if(online_user_login !=  user_sender->getLogin()){
        std::cerr << "Пользователь присылает не верные данные или он не авторизован"  << std::endl;
        // Отправляем ответ об ошибке
        Message50 response;
        response.status_request = false;
        json j;
        response.to_json(j);
        _network->sendMess(j.dump());
        throw std::runtime_error("HandlerMessage4: Закрываю соединение...");
    }

    _db->write_Chat_H(user_sender, m4->mess);
    std::vector<std::vector<std::string>> history_chat_H; 
    _db->load_Chat_H(history_chat_H);
    
    // Отправляем ответ
    Message51 mess_class;
    mess_class.history_chat_H = history_chat_H;
    json mess_json;
    mess_class.to_json(mess_json);
    _network->sendMess(mess_json.dump());
    return true;
}


// Обработчик для Message5 (Запрос на получение списка приватных чатов)
// Если в БД нет данных, то передаем пустоту, пользователь сам должен проверить, что сообщений нет
bool HandlerMessage5::handle(const std::shared_ptr<Message>& message){
    
    if (message->getTupe() != 5) {
        return handleNext(message);
    }

    auto m5 = std::dynamic_pointer_cast<Message5>(message);
    //получаем пользователя и проверяем на nullptr
    std::shared_ptr<User> user_sender = _db->search_User(m5->my_login);

    //есть ли пользователь в базе и логин залогированного пользователя?
    if (user_sender && online_user_login == user_sender->getLogin())
    {
        // получаем данные с БД
        std::vector<std::pair<std::string, std::string>> list_Chat_P = _db->my_chat_P(m5->my_login);
        
        // Отправляем ответ
        Message53 mess_class;
        mess_class.list_chat_P = list_Chat_P;
        json mess_json;
        mess_class.to_json(mess_json);
        _network->sendMess(mess_json.dump());
        return true;
    }
    
    // Отправляем ответ об ошибке
    std::cerr << "Пользователь присылает не верные данные или он не авторизован"  << std::endl;
    // Отправляем ответ об ошибке
    Message50 response;
    response.status_request = false;
    json j;
    response.to_json(j);
    _network->sendMess(j.dump());
    throw std::runtime_error("HandlerMessage5: Закрываю соединение...");
}


// Обработчик для Message6 (Запрос на получение списока юзеров кому написать)
// Если в БД нет данных, то передаем пустоту, пользователь сам должен проверить, что сообщений нет
bool HandlerMessage6::handle(const std::shared_ptr<Message>& message){
    
    if (message->getTupe() != 6) {
        return handleNext(message);
    }

    auto m6 = std::dynamic_pointer_cast<Message6>(message);

    std::shared_ptr<User> user_sender = _db->search_User(m6->my_login);
    
    //есть ли пользователь в базе и логин залогированного пользователя?
    if (user_sender && online_user_login == user_sender->getLogin())
    {
        // получаем данные с БД
        std::vector<std::pair<std::string, std::string>> list_Users = _db->list_all_User(m6->my_login);
        
        // Отправляем ответ
        Message54 mess_class;
        mess_class.list_Users = list_Users;
        json mess_json;
        mess_class.to_json(mess_json);
        _network->sendMess(mess_json.dump());
        return true;
    }
    
    // Отправляем ответ об ошибке
    std::cerr << "Пользователь присылает не верные данные или он не авторизован"  << std::endl;
    // Отправляем ответ об ошибке
    Message50 response;
    response.status_request = false;
    json j;
    response.to_json(j);
    _network->sendMess(j.dump());
    throw std::runtime_error("HandlerMessage6: Закрываю соединение...");
}


// Запрос юзера получить свое имя
bool HandlerMessage7::handle(const std::shared_ptr<Message>& message){
    
    if (message->getTupe() != 7) {
        return handleNext(message);
    }

    auto m7 = std::dynamic_pointer_cast<Message7>(message);
    //получаем пользователя и проверяем на nullptr
    std::shared_ptr<User> user = _db->search_User(m7->my_login);

    //есть ли пользователь в базе и логин залогированного пользователя?
    if (user && online_user_login == user->getLogin())
    {
        // Отправляем ответ
        Message56 mess_class;
        mess_class.my_name = user->getName();
        json mess_json;
        mess_class.to_json(mess_json);
        _network->sendMess(mess_json.dump());
        return true;
    }
    
    // Отправляем ответ об ошибке
    std::cerr << "Ошибка, позможные причины:\n- Пользователь присылает не верные данные\n- Не авторизован\n- Нет данных в БД"  << std::endl;
    // Отправляем ответ об ошибке
    Message50 response;
    response.status_request = false;
    json j;
    response.to_json(j);
    _network->sendMess(j.dump());
    throw std::runtime_error("HandlerMessage7: Закрываю соединение...");
}


// Обработчик для Message8 (обновить данные приватного чата)
bool HandlerMessage8::handle(const std::shared_ptr<Message>& message){
    if (message->getTupe() != 8) {
        return handleNext(message);
    }
    
    auto m8 = std::dynamic_pointer_cast<Message8>(message);
    std::shared_ptr<User> user_sender = _db->search_User(m8->user_sender);
    std::shared_ptr<User> user_recipient = _db->search_User(m8->user_recipient);
    
    if (user_sender == nullptr || user_recipient == nullptr)
    {
        std::cerr << "ошибка бд: HandlerMessage8::handle"  << std::endl;
        //Error: Не верные данные авторизации авторизованного юзера (сообщение 3)
        // Отправляем ответ об ошибке
        Message50 response;
        response.status_request = false;
        json j;
        response.to_json(j);
        _network->sendMess(j.dump());
        throw std::runtime_error("HandlerMessage3: Закрываю соединение...");
    }

    if(online_user_login !=  user_sender->getLogin()){
        std::cerr << "Пользователь присылает не верные данные или он не авторизован"  << std::endl;
        //Error: Попытка получить ответ не авторизованного юзера (сообщение 3)"
        // Отправляем ответ об ошибке
        Message50 response;
        response.status_request = false;
        json j;
        response.to_json(j);
        _network->sendMess(j.dump());
        throw std::runtime_error("HandlerMessage3: Закрываю соединение...");
    }

    std::vector<std::pair<std::string, std::string>> history_chat_P; 
    _db->load_Chat_P(user_sender, user_recipient, history_chat_P);

    // Отправляем ответ
    Message52 mess_class;
    mess_class.history_chat_P = history_chat_P;
    mess_class.login_name_MY.first = user_sender->getLogin();
    mess_class.login_name_MY.second = user_sender->getName();
    mess_class.login_name_friend.first = user_recipient->getLogin();
    mess_class.login_name_friend.second = user_recipient->getName();
    json mess_json;
    json j;
    mess_class.to_json(j);
    _network->sendMess(j.dump());
    
    return true;
}


// Обработчик для Message9 (обновить данные общего чата)
bool HandlerMessage9::handle(const std::shared_ptr<Message>& message){
    
    if (message->getTupe() != 9) {
        return handleNext(message);
    }

    auto m9 = std::dynamic_pointer_cast<Message9>(message);
    std::shared_ptr<User> user_sender = _db->search_User(m9->user_sender);
    
    if (user_sender == nullptr)
    {
        std::cerr << "Error: Не верные данные авторизации авторизованного юзера (сообщение 4)";
        // Отправляем ответ об ошибке
        Message50 response;
        response.status_request = false;
        json j;
        response.to_json(j);
        _network->sendMess(j.dump());
        throw std::runtime_error("HandlerMessage4: Закрываю соединение...");
    }

    if(online_user_login !=  user_sender->getLogin()){
        std::cerr << "Пользователь присылает не верные данные или он не авторизован"  << std::endl;
        // Отправляем ответ об ошибке
        Message50 response;
        response.status_request = false;
        json j;
        response.to_json(j);
        _network->sendMess(j.dump());
        throw std::runtime_error("HandlerMessage4: Закрываю соединение...");
    }

    std::vector<std::vector<std::string>> history_chat_H; 
    _db->load_Chat_H(history_chat_H);
    
    // Отправляем ответ
    Message51 mess_class;
    mess_class.history_chat_H = history_chat_H;
    json mess_json;
    mess_class.to_json(mess_json);
    _network->sendMess(mess_json.dump());
    return true;
}


// Обработчик для неизвестных сообщений
bool HandlerErr::handle(const std::shared_ptr<Message>& message) {
    std::cerr << "Неизвестный тип сообщения: " << message->getTupe() << std::endl;
    
    Message50 response;
    response.status_request = false;
    
    json j;
    response.to_json(j);
    _network->sendMess(j.dump());
    
    return true;
}
