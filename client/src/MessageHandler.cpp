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

MessageHandler::MessageHandler(std::shared_ptr<NetworkClient> network, 
                               std::shared_ptr<interaction_chat> interaction_chat)
    : _network(network), _interaction_chat(interaction_chat), _next(nullptr) 
{
}

bool MessageHandler::handleNext(const std::shared_ptr<Message>& message) {
    if (_next) return _next->handle(message);
    return false;
}

// Обработка для Message50 (авторизован или ошибка)
bool HandlerMessage50::handle(const std::shared_ptr<Message>& message) {
    // Проверяем, наше ли это сообщение
    if (message->getTupe() != 50) {
        // Не наше - передаем следующему в цепочке
        return handleNext(message);
    }
    //обрабатываем
    auto m50 = std::dynamic_pointer_cast<Message50>(message);
    
    if (m50->status_request)
    {
        this->_interaction_chat->setMenu_go_in_Chat(Menu_go_in_Chat::AUTHORIZATION);
    }
    else
    {
        this->_interaction_chat->setMenu_go_in_Chat(Menu_go_in_Chat::VOID_REG);
    }
    return true;
}

// Обработка для Message50 (авторизован или ошибка)
bool HandlerMessage56::handle(const std::shared_ptr<Message>& message) {
    // Проверяем, наше ли это сообщение
    if (message->getTupe() != 56) {
        // Не наше - передаем следующему в цепочке
        return handleNext(message);
    }
    //обрабатываем
    auto m56 = std::dynamic_pointer_cast<Message56>(message);
    this->_interaction_chat->setMyName(m56->my_name);
    std::cout << "Login: " << m56->my_name << std::endl;
    return true;
}