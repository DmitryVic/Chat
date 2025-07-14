#include <string>
#include <iostream>
#include <memory>
#include "Message.h"
#include "UserStatus.h"
#include "NetworkClient.h"
#include "interactive_interface.h"
#include "interaction_chat.h"
#include <nlohmann/json.hpp>
#include "MessageHandler.h"

using json = nlohmann::json;

interaction_chat::interaction_chat(
    std::shared_ptr<NetworkClient> network, 
                   std::shared_ptr<interactive_interface> II, 
                   std::shared_ptr<MessageHandler> hendl,
                   std::shared_ptr<UserStatus> status)
                    : _network(network), _II(II), _hendl(hendl), _status(status) 
{
}


void interaction_chat::start(){
    while (true) {
        
        switch (_status->getMenuAuthoriz())
        {
        case MENU_AUTHORIZATION::EXIT_PROGRAMM:
            return;
        case MENU_AUTHORIZATION::AUTHORIZATION_SUCCESSFUL:{
            this->menu_chat();
            break;}
        case MENU_AUTHORIZATION::AUTHORIZATION:{
            std::shared_ptr<Message1> mes = _II->authorization();
            json j;
            mes->to_json(j);
            _network->sendMess(j.dump());
            this->getMess();



            
            _II->display_message(j.dump());





            // std::shared_ptr<Message7> mes2;
            // mes2->my_login = this->_status->getLogin();
            // json jj;
            // mes->to_json(jj);
            // _network->sendMess(jj.dump());
            // this->getMess();
            break;}
        case MENU_AUTHORIZATION::REG:{
            std::shared_ptr<Message2> mes = _II->reg();
            json j;
            mes->to_json(j);


            _II->display_message(j.dump());


            _network->sendMess(j.dump());
            this->getMess();
            break;}
        case MENU_AUTHORIZATION::VOID_REG:{
            _II->show_menu_authorization(_status);
            break;}
        
        default:
            _status->setMenuAuthoriz(MENU_AUTHORIZATION::EXIT_PROGRAMM);
            return;
        }
    }
}


void interaction_chat::menu_chat() {
    while (true) {
        
        if (_status->getMenuAuthoriz() != MENU_AUTHORIZATION::AUTHORIZATION_SUCCESSFUL)
            return;

        switch (_status->getMenuChat()) {
            case MENU_CHAT::EXIT:
                return;
            case MENU_CHAT::VOID: {
                _II->show_chat_menu(this->_status);
                break;
            }
            case MENU_CHAT::SHOW_CHAT_H: {
                //Запрос на отображение общего чата, обработчик должен открыть общий чат получив ответ сервера с обновленными
                // данными, II поменяв MENU_CHAT в SHOW_CHAT_H или в 50 ответе поменяется на выход, что значит не верные данные
                    std::shared_ptr<Message9> mes = std::make_shared<Message9>();
                    mes->user_sender = this->_status->getLogin();
                    json j;
                    mes->to_json(j);
                    _network->sendMess(j.dump());
                    this->getMess();
                break;
            }
            case MENU_CHAT::LIST_CHAT_P: {
                std::shared_ptr<Message5> mes = std::make_shared<Message5>();
                mes->my_login = this->_status->getLogin();
                json j;
                mes->to_json(j);
                _network->sendMess(j.dump());
                this->getMess();
                break;
            }
            case MENU_CHAT::LIST_USERS: {
                std::shared_ptr<Message6> mes = std::make_shared<Message6>();
                json j;
                mes->to_json(j);
                _network->sendMess(j.dump());
                this->getMess();
                break;
            }
            default:
                _status->setMenuAuthoriz(MENU_AUTHORIZATION::EXIT_PROGRAMM);
                return;
            }
    }
}


void interaction_chat::getMess() {
    try {
        if (!_hendl) {
            throw std::runtime_error("Обработчик сообщений не инициализирован");
        }
        
        std::string json_str = _network->getMess();
        _II->display_message(json_str);

        auto msg = parse_message(json_str);
        
        if (!msg) {
            throw std::runtime_error("Неверное сообщение с сервера");
        }
        
        if (!_hendl->handle(msg)) {
            throw std::runtime_error("Обработка сообщений не удалась");
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        _II->display_message(e.what());
    }
}