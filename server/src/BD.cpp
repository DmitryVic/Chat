#include "File.h"
#include "User.h"
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>


namespace fs = std::filesystem;

static std::string BD_FILE = "file/DB.txt";
//путь к файлу общего чата, путь к БД находится в БД cpp file/
static std::string CHAT_HARED_FILE = "file/ChH.txt";


/*=====================================
        ОБЩИЕ ФУНКЦИИ
=====================================*/


// Файл существует или нет
bool fileExists(const std::string& filename) {
    std::ifstream file(filename);
    return file.good();
}


// Создает директорию, если её нет
void createDirectoryIfNeeded(const std::string& path) {

    auto dir_path = fs::path(path).parent_path();
    if (!dir_path.empty() && !fs::exists(dir_path)) {
        fs::create_directories(dir_path);
    }

}


// Создать файл
void createFile(const std::string& filename) {
    createDirectoryIfNeeded(filename);

    std::ofstream file(filename, std::ios::out);

    if (file.is_open()) {
        file.close();
    } else {
        std::cerr << "Не удалось создать файл " << filename << std::endl;
    }
}


// Список всех .txt-файлов в директории dir
std::vector<std::string> listChatFiles(const std::string& dir) {
    std::vector<std::string> out;
    if (!fs::exists(dir) || !fs::is_directory(dir)) return out;
    for (auto& p : fs::directory_iterator(dir)) {
        if (p.is_regular_file() && p.path().extension() == ".txt") {
            out.push_back(p.path().string());
        }
    }
    return out;
}


//=================================================================
// БД: запись и чтение структур
//=================================================================


//запись юзера
void BD_write_User(std::shared_ptr<User> user) {
    createDirectoryIfNeeded(BD_FILE);
    std::ofstream fs(BD_FILE, std::ios::app);
    if (fs.is_open()) {
        fs << user->getLogin() << '\n'
           << user->getPass() << '\n'
           << user->getName() << "\n\n";
        fs.close();
    } else {
        std::cerr << "Ошибка записи в файл БД: " << BD_FILE << std::endl;
    }
}


//Поиск юзера пологину
std::shared_ptr<User> BD_search_User(const std::string& log){
    createDirectoryIfNeeded(BD_FILE);
    if(!fileExists(BD_FILE)){
        return nullptr;
    }

    std::ifstream fr(BD_FILE);
    if(!fr.is_open()) {
        std::cerr << "Не могу открыть файл " << BD_FILE << '\n';
        return nullptr;
    }

    std::string login;
    std::string password;
    std::string name;
    
    while (std::getline(fr, login) && 
           std::getline(fr, password) && 
           std::getline(fr, name)) {
        // Пропускаем пустые строки
        // Если любое из полей пустое - пропускаем эту итерацию цикла Защита от некорр данных
        if (login.empty() || password.empty() || name.empty()) {
            continue;
        }
        
        if (login == log)
        {
            fr.close();
            return std::make_shared<User>(login, password, name);
        }
        
        // Пропускаем пустую строку-разделитель
        std::string empty_line;
        std::getline(fr, empty_line);
    }

    fr.close();
    return nullptr;
}



/*=====================================
        ФУНКЦИИ ЧАТОВ
=====================================*/


//запись в приватный чат, проверить перед записью существоваие файла!
void write_Chat_P(const std::string& filename, std::shared_ptr<User> user, const std::string& mess) {
    if (!fileExists(filename)) return;
    std::ofstream fs(filename, std::ios::app);
    if (fs.is_open()) {
        fs << user->getLogin() << '\n'
           << mess << "\n\n";
        fs.close();
    } else {
        std::cerr << "Ошибка записи в файл чата: " << filename << std::endl;
    }
}


// Загрузить историю приватного чата: пары <login, сообщение>
bool load_Chat_P(const std::string& filename, std::vector<std::pair<std::string, std::string>>& out) {
    if (!fileExists(filename)) return false;

    std::ifstream fs(filename);

    if (!fs.is_open()) return false;

    std::string login, mess;
    while (std::getline(fs, login) && std::getline(fs, mess)) {
        out.emplace_back(login, mess);
        std::string sep;
        std::getline(fs, sep);
    }
    return true;
}


// //запись в общий чат, проверить перед записью существоваие файла!
// void write_Chat_H(const std::string& filename, std::shared_ptr<User> user, const std::string& mess) {
    
// }


// // Загрузить историю общего чата: пары <login, сообщение>
// bool load_Chat_H(const std::string& filename, std::vector<std::pair<std::string, std::string>>& out) {

// }