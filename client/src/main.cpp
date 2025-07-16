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
//локаль
#include <locale>
#include <clocale>

#ifdef _WIN32
    #include <windows.h>
#endif


using namespace std;
using json = nlohmann::json;

#define PORT 7777

int main() {
    
    // Универсальная настройка локали
    setlocale(LC_ALL, "ru_RU.UTF-8");

    // Для Windows
    #ifdef _WIN32
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    #endif

    // Для Linux
    #ifdef SET_GLOBAL_LOCALE_LINUX
    try {
        std::locale::global(std::locale("ru_RU.UTF-8"));
    } catch (const std::exception& e) {
        std::cerr << "Locale error: " << e.what() << std::endl;
        std::locale::global(std::locale("C.UTF-8")); // Fallback
    }
    #endif
    
    std::shared_ptr<NetworkClient> network = std::make_shared<NetworkClient>("127.0.0.1", PORT);
    network->connecting();

    std::shared_ptr<interactive_interface> II = std::make_shared<console_interface>();
    std::shared_ptr<UserStatus> status = std::make_shared<UserStatus>();
    
    // Создаем обработчики
    auto Handler50 = std::make_shared<HandlerMessage50>(network, II, status);
    auto Handler51 = std::make_shared<HandlerMessage51>(network, II, status);
    auto Handler52 = std::make_shared<HandlerMessage52>(network, II, status);
    auto Handler53 = std::make_shared<HandlerMessage53>(network, II, status);
    auto Handler54 = std::make_shared<HandlerMessage54>(network, II, status);
    auto Handler55 = std::make_shared<HandlerMessage55>(network, II, status);
    auto Handler56 = std::make_shared<HandlerMessage56>(network, II, status);
    auto messageError = std::make_shared<HandlerErr>(network, II, status);
    // Связываем
    messageError->setNext(nullptr);
    Handler56->setNext(messageError);
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