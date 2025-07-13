#pragma once
#include <string>
#include "User.h"
#include <memory>
#include "UserStatus.h"
#include "NetworkClient.h"
#include "interactive_interface.h"
#include "MessageHandler.h"

class interaction_chat
{
private:
    std::shared_ptr<NetworkClient> _network;
    std::shared_ptr<interactive_interface> _II;
    Menu_go_in_Chat menu_authorization = Menu_go_in_Chat::VOID_REG;
    std::shared_ptr<User> _myUser;
    std::shared_ptr<MessageHandler> _hendl;
    
    void menu_authoriz();
    void menu_chat();

public:
    interaction_chat(std::shared_ptr<NetworkClient> network, 
                   std::shared_ptr<interactive_interface> II, 
                   std::shared_ptr<User> myUser,
                   std::shared_ptr<MessageHandler> hendl);
    
    ~interaction_chat() = default;
    
    void start();
    void setMyName(std::string& name);
    void getMess();
    void setMessageHandler(std::shared_ptr<MessageHandler> handler);
    void setMenu_go_in_Chat(Menu_go_in_Chat menu);
};