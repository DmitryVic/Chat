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
    std::shared_ptr<User> user = std::make_shared<User>(" ", " ", " ");
    std::shared_ptr<interactive_interface> II = std::make_shared<console_interface>();
    
    // Создаем interaction_chat сначала с nullptr для обработчика
    auto my_chat = std::make_shared<interaction_chat>(network, II, user, nullptr);
    
    // Теперь создаем обработчики
    auto Handler50 = std::make_shared<HandlerMessage50>(network, my_chat);
    auto Handler56 = std::make_shared<HandlerMessage56>(network, my_chat);
    
    Handler50->setNext(Handler56);
    
    // Устанавливаем обработчики в interaction_chat
    my_chat->setMessageHandler(Handler50);
    
    my_chat->start();
    return 0;
}