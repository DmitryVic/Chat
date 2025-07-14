#include "MessageHandler.h"
#include "NetworkClient.h"
#include "Message.h"
#include <nlohmann/json.hpp>
#include <string>
#include <variant>
#include <memory>
#include "interaction_chat.h"
#include "UserStatus.h"
#include "MessageHandler.h"
#include "interaction_chat.h" // Теперь включаем здесь

// MessageHandler::MessageHandler(std::shared_ptr<NetworkClient> network, std::shared_ptr<interactive_interface> II, std::shared_ptr<UserStatus> status)
//     : _network(network), _II(II), _status(status), _next(nullptr) 
// {
// }

void MessageHandler::getMess(){
    try {
       
        std::string json_str = _network->getMess();
        auto msg = parse_message(json_str);
        
        if (!msg) {
            throw std::runtime_error("Неверное сообщение с сервера");
        }
        
        std::cerr << "Обработка сообщения в MessageHandler::getMess" << std::endl;
        this->handle(msg);
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        _II->display_message(e.what());
    }
}

bool MessageHandler::handleNext(const std::shared_ptr<Message>& message) {
    if (_next) return _next->handle(message);
    return false;
}

// Обработка для Message50 (авторизован или ошибка)
bool HandlerMessage50::handle(const std::shared_ptr<Message>& message) {
    std::cerr << &message << "\n";
    // Проверяем, наше ли это сообщение
    if (message->getTupe() != 50) {
        // Не наше - передаем следующему в цепочке
        return handleNext(message);
    }
    //обрабатываем
    auto m50 = std::dynamic_pointer_cast<Message50>(message);
    
    if (m50->status_request)
    {
        _status->setMenuAuthoriz(MENU_AUTHORIZATION::AUTHORIZATION_SUCCESSFUL);
        std::shared_ptr<Message7> mes = std::make_shared<Message7>();
        mes->my_login = this->_status->getLogin();
        json jj;
        mes->to_json(jj);
        _network->sendMess(jj.dump());
        // this->getMess();
    }
    else
    {
    _status->setMenuAuthoriz(MENU_AUTHORIZATION::VOID_REG);
    _status->setMenuChat(MENU_CHAT::VOID);
    _status->setLogin("");
    _status->setPass("");
    _status->setName("");
    _II->display_message("Ошибка в обмене ");
    }
    return true;
}


// Обработка для Message51 (Передача данных общего чата)
bool HandlerMessage51::handle(const std::shared_ptr<Message>& message) {
    std::cerr << &message << "\n";
    // Проверяем, наше ли это сообщение
    if (message->getTupe() != 51) {
        // Не наше - передаем следующему в цепочке
        return handleNext(message);
    }
    //обрабатываем
    auto m51 = std::dynamic_pointer_cast<Message51>(message);

    std::shared_ptr<Message4> data = _II->show_chat_H(m51->history_chat_H, _status->getName(), _status);
    if ( data->mess == "")
    {
        _status->setMenuChat(MENU_CHAT::VOID);
    }
    else{
        std::shared_ptr<Message4> mes = std::make_shared<Message4>();
        mes->user_sender = this->_status->getLogin();
        mes->mess = data->mess;
        json jj;
        mes->to_json(jj);
        _network->sendMess(jj.dump());
        this->getMess();
    }
    return true;
}

// Обработка для Message52 (Передача данных приватного чата)
bool HandlerMessage52::handle(const std::shared_ptr<Message>& message) {
    std::cerr << &message << "\n";
    // Проверяем, наше ли это сообщение
    if (message->getTupe() != 52) {
        // Не наше - передаем следующему в цепочке
        return handleNext(message);
    }
    //обрабатываем
    auto m52 = std::dynamic_pointer_cast<Message52>(message);
    std::shared_ptr<Message3> data = _II->show_chat_P(m52->history_chat_P, _status->getName(), "Доработать в status", _status);

    if ( data->mess == "")
    {
        _status->setMenuChat(MENU_CHAT::LIST_CHAT_P);
    }
    else{
        _status->setMenuChat(MENU_CHAT::SHOW_CHAT_P);
        std::shared_ptr<Message3> mes = std::make_shared<Message3>();
        mes->user_sender = this->_status->getLogin();
        mes->mess = data->mess;
        json jj;
        mes->to_json(jj);
        _network->sendMess(jj.dump());
        this->getMess();
    }

    return true;
}




// Обработка для Message53 (Передача списка истории приватных чатов)
bool HandlerMessage53::handle(const std::shared_ptr<Message>& message) {
    std::cerr << &message << "\n";
    // Проверяем, наше ли это сообщение
    if (message->getTupe() != 53) {
        // Не наше - передаем следующему в цепочке
        return handleNext(message);
    }
    //обрабатываем
    auto m53 = std::dynamic_pointer_cast<Message53>(message);
    std::pair<std::string, std::string> data = _II->show_list_chat_P(m53->list_chat_P, _status);
    if ( data.first == "" || data.second == "")
    {
        _status->setMenuChat(MENU_CHAT::VOID);
    }
    else{
        std::shared_ptr<Message8> mes = std::make_shared<Message8>();
        mes->user_sender = this->_status->getLogin();
        mes->user_recipient = data.first;
        json jj;
        mes->to_json(jj);
        _network->sendMess(jj.dump());
        this->getMess();
    }
    return true;
}








// Обработка для Message53 (получить список всех юзеров в чате кому написать)
bool HandlerMessage54::handle(const std::shared_ptr<Message>& message) {
    std::cerr << &message << "\n";
    // Проверяем, наше ли это сообщение
    if (message->getTupe() != 54) {
        // Не наше - передаем следующему в цепочке
        return handleNext(message);
    }
    //обрабатываем
    auto m54 = std::dynamic_pointer_cast<Message54>(message);
    //ответ и открытие чата
    std::pair<std::string, std::string> data = _II->show_list_users(m54->list_Users, _status);
    if ( data.first == "" || data.second == "")
    {
        _status->setMenuChat(MENU_CHAT::VOID);
    }
    else{
        std::shared_ptr<Message8> mes = std::make_shared<Message8>();
        mes->user_sender = this->_status->getLogin();
        mes->user_recipient = data.first;
        json jj;
        mes->to_json(jj);
        _network->sendMess(jj.dump());
        this->getMess();
    }
    return true;
}


// Обработка для Message53 (Ответ сервера логин занят)
bool HandlerMessage55::handle(const std::shared_ptr<Message>& message) {
    std::cerr << &message << "\n";
    // Проверяем, наше ли это сообщение
    if (message->getTupe() != 55) {
        // Не наше - передаем следующему в цепочке
        return handleNext(message);
    }
    //обрабатываем
    _status->setMenuAuthoriz(MENU_AUTHORIZATION::VOID_REG);
    _status->setMenuChat(MENU_CHAT::VOID);
    _status->setLogin("");
    _status->setPass("");
    _status->setName("");
    _II->display_message("Данный логин занят!");
    return true;
}


// Обработка для Message56 (Ответ сервера вернуть имя)
bool HandlerMessage56::handle(const std::shared_ptr<Message>& message) {
    std::cerr << message << "\n";
    // Проверяем, наше ли это сообщение
    if (message->getTupe() != 56) {
        // Не наше - передаем следующему в цепочке
        return handleNext(message);
    }
    //обрабатываем
    auto m56 = std::dynamic_pointer_cast<Message56>(message);
    this->_status->setLogin(m56->my_name);
    std::string hi = m56->my_name;
    hi += ", здраствуйте!";
    _II->display_message(hi);
    return true;
}

// Обработка для Message56 (Ответ сервера вернуть имя)
bool HandlerErr::handle(const std::shared_ptr<Message>& message) {
    std::cerr << message << "\n";
    //обрабатываем
    _status->setMenuAuthoriz(MENU_AUTHORIZATION::VOID_REG);
    _status->setMenuChat(MENU_CHAT::VOID);
    _status->setLogin("");
    _status->setPass("");
    _status->setName("");
    _II->display_message("Ошибка ответа сервера");
    return true;
}