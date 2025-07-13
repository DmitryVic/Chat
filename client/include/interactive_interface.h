#pragma once
#include <string>
#include <vector>
#include <utility>
#include <Message.h>
#include "User.h"
#include <memory>

enum Menu_Chat {LIST_CHAT_P, LIST_USERS, SHOW_CHAT_H, EXIT}; 
Menu_Chat menu;

enum Menu_go_in_Chat {AUTHORIZATION, REG, EXIT}; 
Menu_go_in_Chat menu_authorization;

class interactive_interface
{
private:
    /* data */
public:
    interactive_interface(){};
    virtual ~interactive_interface() = default;

    // отобразить чаты предать историю и 
    virtual std::shared_ptr<Message3> show_chat_P(const std::vector<std::pair<std::string, std::string>>& history_chat_P, 
        const std::string& my_User, const  std::string& friend_User) = 0;
        
    virtual std::shared_ptr<Message4> show_chat_H(const  std::vector<std::pair<std::string, std::string>>& history_chat_H, 
        const std::string& my_User) = 0;

    // отобразить поле авторизации
    virtual  std::shared_ptr<Message1> authorization() = 0;

    // отобразить поле регистрации логин
    virtual  std::shared_ptr<Message2> reg() = 0;

    // отобразить список приватных чатов
    virtual  std::pair<std::string, std::string> show_list_chat_P(const std::vector<std::pair<std::string, const std::string>>& list_Chat_P) = 0;

    // отобразить пользователей кому написать
    virtual  std::pair<std::string, std::string> show_list_users(const std::vector<std::pair<std::string, const std::string>>& list_Users) = 0;

    //отобразить меню
    virtual Menu_Chat show_chat_menu() = 0;

    //отобразить авторизации
    virtual Menu_go_in_Chat show_menu_authorization() = 0;

    //отобразить авторизации
    virtual void no_connect() = 0;

    //для отображения системных сообщений
    virtual void display_message(const  std::string& info) = 0;
};

