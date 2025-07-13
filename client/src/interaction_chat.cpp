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
    std::shared_ptr<User> myUser,
    std::shared_ptr<MessageHandler> hendl)
    : _network(network), _II(II), _myUser(myUser), _hendl(hendl) 
{
}

void interaction_chat::start(){
    while (true) {
        if (this->menu_authorization == Menu_go_in_Chat::EXIT_PROGRAMM) {
            break;
        }
        else if (menu_authorization == Menu_go_in_Chat::AUTHORIZATION_SUCCESSFUL) {
            this->menu_chat();
            this->getMess();
        }
        else {
            this->menu_authoriz();
            this->getMess();
        }
    }
}

void interaction_chat::menu_authoriz() {
    while (true) {
        this->menu_authorization = _II->show_menu_authorization();

        switch (this->menu_authorization) {
        case Menu_go_in_Chat::AUTHORIZATION: {
            auto my_mess = _II->authorization();
            _myUser->setLogin(my_mess->login);
            _myUser->setPass(my_mess->pass);
            
            json j;
            my_mess->to_json(j);
            _network->sendMess(j.dump());
            return;
        }
        case Menu_go_in_Chat::REG: {
            auto my_mess = _II->reg();
            _myUser->setLogin(my_mess->login);
            _myUser->setPass(my_mess->pass);
            _myUser->setName(my_mess->name);
            
            json j;
            my_mess->to_json(j);
            _network->sendMess(j.dump());
            return;
        }
        default:
            this->menu_authorization = Menu_go_in_Chat::EXIT_PROGRAMM;
            return;
        }
    }
}



void interaction_chat::menu_chat() {
    // Реализация меню чата
    std::cout << "Chat menu" << std::endl;
    //ДОПИСАТЬ МЕТОД !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!






}

void interaction_chat::setMyName(std::string& name) {
    _myUser->setName(name);
}

void interaction_chat::setMessageHandler(std::shared_ptr<MessageHandler> handler) {
    _hendl = handler;
}

void interaction_chat::getMess() {
    try {
        if (!_hendl) {
            throw std::runtime_error("Обработчик сообщений не инициализирован");
        }
        
        std::string json_str = _network->getMess();
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

void interaction_chat::setMenu_go_in_Chat(Menu_go_in_Chat menu) {
    menu_authorization = menu;
}