#include "MessageHandler.h"
#include "NetworkClient.h"
#include "Message.h"
#include <nlohmann/json.hpp>
#include <string>
#include <variant>
#include <memory>


bool MessageHandler::handleNext(const std::shared_ptr<Message>& message) {
    if (_next) return _next->handle(message);
    return false;
}

// // Обработка для Message1 (авторицация)
// bool HandlerMessage1::handle(const std::shared_ptr<Message>& message) {
//     // Проверяем, наше ли это сообщение
//     if (message->getTupe() != 1) {
//         // Не наше - передаем следующему в цепочке
//         return handleNext(message);
//     }
//     //обрабатываем
//     auto m1 = std::dynamic_pointer_cast<Message1>(message);
    
//     // Логика обработки
//     std::shared_ptr<User> user = _db->search_User(m1->login);

//     bool authSuccess = user && (hashPassword(m1->pass) == user->getPass());
//     //Фиксация авторизации
//     if (authSuccess)
//         online_user_login = user->getLogin();
    

//     // Формируем ответ
//     Message50 response;
//     response.status_request = authSuccess;
    
//     // Отправляем ответ через сеть
//     json j;
//     response.to_json(j);
//     _network->sendMess(j.dump());
    
//     return true;  // Сообщение обработано
// }