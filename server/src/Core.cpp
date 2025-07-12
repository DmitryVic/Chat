#include "Core.h"
#include "log.h"
#include "NetworkServer.h"
#include "MessageHandler.h"
#include "BD.h"
#include "Message.h"
#include <iostream>
#include <nlohmann/json.hpp>
#include <memory>


void chat_start(std::shared_ptr<DataBase> db, 
               std::shared_ptr<NetworkServer> network) {
                
    network->acceptClient();
    // Создаем обработчики
    auto Handler1 = std::make_unique<HandlerMessage1>(db, network);
    auto Handler2 = std::make_unique<HandlerMessage2>(db, network);
    auto Handler3 = std::make_unique<HandlerMessage3>(db, network);
    auto messageError = std::make_unique<HandlerErr>(db, network);

    // Строим цепочку С КОНЦА:  В ДРУГОМ ПОРЯДКЕ НЕЛЬЗЯ move ПЕРЕДАЕТ ВЛАДЕНИЕ ПОСЛЕ 1 ПЕРЕМЕШЕНИЯ ПЕРЕДАДИМ NULLPTR 
    messageError->setNext(nullptr);  

    Handler3->setNext(std::move(messageError));// Последний в цепочке messageError
    // NEW

    Handler2->setNext(std::move(Handler3));
    Handler1->setNext(std::move(Handler2));



    while (true) {
        try {
            std::string json_str = network->getMess();
            auto msg = parse_message(json_str);
            
            if (!msg) {
                Message50 mess_err;
                mess_err.status_request = false;
                json mess_json;
                mess_err.to_json(mess_json);
                std::string mess_push = mess_json.dump();
                network->sendMess(mess_push);
                
                continue;
            }
            
            if (!Handler1->handle(msg)) {
                Message50 mess_err;
                mess_err.status_request = false;
                json mess_json;
                mess_err.to_json(mess_json);
                std::string mess_push = mess_json.dump();
                network->sendMess(mess_push);
            }
            
        } catch (const std::exception& e) {
            std::cout << e.what();
            break;
        }
    }
}
