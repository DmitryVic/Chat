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
#include <fstream>
#include <filesystem>

using namespace std;
using json = nlohmann::json;

#define PORT 7777

std::string LOG_F = "file/log.txt";
namespace fs = std::filesystem;

int main() {
    
    // Универсальная настройка локали
    setlocale(LC_ALL, "ru_RU.UTF-8");

    // Для Linux
    #ifdef SET_GLOBAL_LOCALE_LINUX
    try {
        std::locale::global(std::locale("ru_RU.UTF-8"));
    } catch (const std::exception& e) {
        std::cerr << "Locale error: " << e.what() << std::endl;
        std::locale::global(std::locale("C.UTF-8")); // Fallback
    }
    #endif

    auto dir_path = std::filesystem::path(LOG_F).parent_path();
    if (!dir_path.empty() && !std::filesystem::exists(dir_path)) {
        std::filesystem::create_directories(dir_path);
    }

    // Сохраняем оригинальный буфер cerr
    std::streambuf* original_cerr_buf = std::cerr.rdbuf();
    
    std::ofstream log(LOG_F);
    if (!log) {
        std::cerr << "Ошибка открытия файла!\n";
        return 1;
    }

    // Перенаправляем cerr в файл
    std::cerr.rdbuf(log.rdbuf());

    std::shared_ptr<DataBase> db = make_shared<DataBaseFile>();
    std::shared_ptr<NetworkServer> network = std::make_shared<NetworkServer>(PORT);
    network->start();
    try {
        
        chat_start(db, network);

    } catch (const exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
    }

    // Восстанавливаем стандартный поток логов
    std::cerr.rdbuf(original_cerr_buf);

    return 0;
}