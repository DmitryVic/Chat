#include "NetworkServer.h"
#include "BD.h"
#include "Message.h"
#include "Core.h"
#include <iostream>
#include <unistd.h>             //базовые функции для работы с системой Linux
#include <string.h>             //библиотека для работы со строками C
#include <sys/socket.h>         //для работы с сокетами
#include <netinet/in.h>         //содержит структуры и константы для работы с протоколами
#include <nlohmann/json.hpp>
#include <memory>

using namespace std;
using json = nlohmann::json;

#define PORT 7777

int main() {
    
    std::shared_ptr<DataBase> db = make_shared<DataBaseFile>();
    std::shared_ptr<NetworkServer> network = std::make_shared<NetworkServer>(PORT);
    network->start();
    try {
        
        chat_start(db, network);

    } catch (const exception& e) {
        cerr << "Ошибка: " << e.what() << endl;
    }
    return 0;
}