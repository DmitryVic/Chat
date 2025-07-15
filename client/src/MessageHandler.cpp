#include "MessageHandler.h"
#include "NetworkClient.h"
#include "Message.h"
#include <nlohmann/json.hpp>
#include <string>
#include <variant>
#include <utility>
#include <memory>
#include "interaction_chat.h"
#include "UserStatus.h"
#include "MessageHandler.h"
#include "interaction_chat.h" // Теперь включаем здесь

// MessageHandler::MessageHandler(std::shared_ptr<NetworkClient> network, std::shared_ptr<interactive_interface> II, std::shared_ptr<UserStatus> status)
//     : _network(network), _II(II), _status(status), _next(nullptr) 
// {
// }

// void MessageHandler::getMess(){
//     try {
       
//         std::string json_str = _network->getMess();
//         auto msg = parse_message(json_str);
        
//         if (!msg) {
//             throw std::runtime_error("Неверное сообщение с сервера");
//         }
//         ///////////////////////////////////////////////////////////////////////////////////////
//         std::cerr << "MessageHandler::getMess" << std::endl;
//         _II->display_message(json_str);
//         std::cerr << "Обработка сообщения в MessageHandler::getMess" << std::endl;
//         ///////////////////////////////////////////////////////////////////////////////////////
//         this->handle(msg);
        
//     } catch (const std::exception& e) {
//         std::cerr << "Error: " << e.what() << std::endl;
//         _II->display_message(e.what());
//         _status->setMenuAuthoriz(MENU_AUTHORIZATION::EXIT_PROGRAMM);
//     }
// }

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
    _status->setMess(message);
    _status->setMessType(51);
    _status->set_message_status(true);
    _status->setMenuChat(MENU_CHAT::SHOW_CHAT_H);
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
    _status->setMess(message);
    _status->setMessType(52);
    _status->set_message_status(true);
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
    _status->setMess(message);
    _status->setMessType(53);
    _status->set_message_status(true);
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
    _status->setMess(message);
    _status->setMessType(54);
    _status->set_message_status(true);
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
    this->_status->setName(m56->my_name);
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