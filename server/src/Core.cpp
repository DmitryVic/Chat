#include "Core.h"
#include "NetworkServer.h"
#include "MessageHandler.h"
#include "BD.h"
#include "Message.h"
#include <iostream>
#include <nlohmann/json.hpp>
#include <memory>

std::string online_user_login;

void chat_start(std::shared_ptr<DataBase> db, 
               std::shared_ptr<NetworkServer> network) {
                
    network->acceptClient();
    // Создаем обработчики
    auto Handler1 = std::make_unique<HandlerMessage1>(db, network);
    auto Handler2 = std::make_unique<HandlerMessage2>(db, network);
    auto Handler3 = std::make_unique<HandlerMessage3>(db, network);
    auto Handler4 = std::make_unique<HandlerMessage4>(db, network);
    auto Handler5 = std::make_unique<HandlerMessage5>(db, network);
    auto Handler6 = std::make_unique<HandlerMessage6>(db, network);
    auto Handler7 = std::make_unique<HandlerMessage7>(db, network);
    auto Handler8 = std::make_unique<HandlerMessage8>(db, network);
    auto Handler9 = std::make_unique<HandlerMessage9>(db, network);
    auto messageError = std::make_unique<HandlerErr>(db, network);

    // Строим цепочку С КОНЦА:  В ДРУГОМ ПОРЯДКЕ НЕЛЬЗЯ move ПЕРЕДАЕТ ВЛАДЕНИЕ ПОСЛЕ 1 ПЕРЕМЕШЕНИЯ ПЕРЕДАДИМ NULLPTR 
    messageError->setNext(nullptr);  

    Handler9->setNext(std::move(messageError));// Последний в цепочке messageError
    // NEW
    Handler8->setNext(std::move(Handler9));
    Handler7->setNext(std::move(Handler8));
    Handler6->setNext(std::move(Handler7));
    Handler5->setNext(std::move(Handler6));
    Handler4->setNext(std::move(Handler5));
    Handler3->setNext(std::move(Handler4));
    Handler2->setNext(std::move(Handler3));
    Handler1->setNext(std::move(Handler2));



    while (true) {
        try {
            std::string json_str = network->getMess();
            auto msg = parse_message(json_str);
            
            if (!msg)
                throw  std::runtime_error("Ошибка в полученых данных от пользователя, закрываю соединение...");
            
            if (!Handler1->handle(msg))
                throw  std::runtime_error("Ошибка в обработке данных, закрываю соединение...");
            
        } catch (const std::exception& e) {
            std::cerr << e.what();
            break;
        }
    }
}
