#include "Message.h"
#include "NetworkClient.h"
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <nlohmann/json.hpp>
#include <string>
#include <memory>
#include "UserStatus.h"
#include "interactive_interface.h"
#include "console_interface.h"
#include "interaction_chat.h"
#include <nlohmann/json.hpp>
#include "User.h"
#include "MessageHandler.h"

using namespace std;
using json = nlohmann::json;

#define PORT 7777

int main() {
    std::shared_ptr<NetworkClient> network = std::make_shared<NetworkClient>("127.0.0.1", PORT);
    network->connecting();
    std::shared_ptr<interactive_interface> II = std::make_shared<console_interface>();
    std::shared_ptr<UserStatus> status = std::make_shared<UserStatus>();
    
    
    
    // Теперь создаем обработчики
    auto Handler50 = std::make_shared<HandlerMessage50>(network, II, status);
    auto Handler51 = std::make_shared<HandlerMessage51>(network, II, status);
    auto Handler52 = std::make_shared<HandlerMessage52>(network, II, status);
    auto Handler53 = std::make_shared<HandlerMessage53>(network, II, status);
    auto Handler54 = std::make_shared<HandlerMessage54>(network, II, status);
    auto Handler55 = std::make_shared<HandlerMessage55>(network, II, status);
    auto Handler56 = std::make_shared<HandlerMessage56>(network, II, status);
    auto messageError = std::make_shared<HandlerErr>(network, II, status);
    
    Handler56->setNext(nullptr);
    Handler55->setNext(Handler56);
    Handler54->setNext(Handler55);
    Handler53->setNext(Handler54);
    Handler52->setNext(Handler53);
    Handler51->setNext(Handler52);
    Handler50->setNext(Handler51);
    
    auto my_chat = std::make_shared<interaction_chat>(network, II, Handler50, status);
    
    my_chat->start();
    return 0;
}