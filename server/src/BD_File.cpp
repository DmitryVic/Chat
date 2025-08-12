#include "BD.h"
#include "BD_File.h"
#include "User.h"
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;



DataBaseFile::DataBaseFile()
{
    //Создть директорию, если её нет
    createDirectoryIfNeeded(BD_FILE);
    //Если существует такой файл
    if (!fileExists(BD_FILE)) {
        //Нету такого файла создаем заного
        createFile(BD_FILE);
    }
    //Создть директорию, если её нет
    createDirectoryIfNeeded(CHAT_HARED_FILE);
    //Если существует такой файл
    if (!fileExists(CHAT_HARED_FILE)) {
        //Нету такого файла создаем заного
        createFile(CHAT_HARED_FILE);
    }
}

DataBaseFile::~DataBaseFile() = default;

/*=====================================
        ОБЩИЕ ФУНКЦИИ
=====================================*/


// Файл существует или нет
bool DataBaseFile::fileExists(const std::string& filename) {
    std::ifstream file(filename);
    return file.good();
}


// Создает директорию, если её нет
void DataBaseFile::createDirectoryIfNeeded(const std::string& path) {

    auto dir_path = fs::path(path).parent_path();
    if (!dir_path.empty() && !fs::exists(dir_path)) {
        fs::create_directories(dir_path);
    }

}


// Создать файл
void DataBaseFile::createFile(const std::string& filename) {
    createDirectoryIfNeeded(filename);

    std::ofstream file(filename, std::ios::out);

    if (file.is_open()) {
        file.close();
    } else {
        std::cerr << "Не удалось создать файл " << filename << std::endl;
    }
}


// Список всех .txt-файлов в директории dir
std::vector<std::string> DataBaseFile::listChatFiles(const std::string& dir) {
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
void DataBaseFile::write_User(std::shared_ptr<User> user) {
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
std::shared_ptr<User> DataBaseFile::search_User(const std::string& log){
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


// Получить список всех юзеров исключая юзера, который делает запрос
std::vector<std::pair<std::string, std::string>> DataBaseFile::list_all_User(std::string my_login){
    createDirectoryIfNeeded(BD_FILE);
    std::vector<std::pair<std::string, std::string>> send;
    if(!fileExists(BD_FILE)){
        return send;
    }

    std::ifstream fr(BD_FILE);
    if(!fr.is_open()) {
        std::cerr << "Не могу открыть файл " << BD_FILE << '\n';
        return send;
    }

    std::string login;
    std::string password;
    std::string name;
    
    while (std::getline(fr, login) && 
           std::getline(fr, password) && 
           std::getline(fr, name)) {
        // Пропускаем пустые строки
        // Если любое из полей пустое - пропускаем эту итерацию цикла Защита от некорр данных
        if (login.empty() || password.empty() || name.empty())
            continue;
        
        // Пропускаем пустую строку-разделитель
        std::string empty_line;
        std::getline(fr, empty_line);

        // пропускаем нашего пользователя
        if (login == my_login)
            continue;

        std::shared_ptr<User> temporary_user = std::make_shared<User>(login, password, name);

        send.push_back({temporary_user->getLogin(), temporary_user->getName()});
        
        
    }

    fr.close();
    return send;
}


//получение бзеров с кем есть приватный чат, возвращает логины и имена для отображения
// std::vector<std::pair<std::string ЛОГИН, std::string ИМЯ>>
std::vector<std::pair<std::string, std::string>> DataBaseFile::my_chat_P(std::string my_login) {
    std::vector<std::pair<std::string, std::string>> send;
    // файлы лежат в "file/"
    //Список путей  к файлам в папке file , path объект пути
    for (auto& path : listChatFiles("file")) {
        // файл: ".../file/login1_login2.txt"
        // имя файла без расширения .txt - строка
        auto fname = std::filesystem::path(path).stem().string(); 
        // позиция симпола _
        auto pos = fname.find('_');
        // нету символа _  - пропускаем
        if (pos == std::string::npos) continue;

        //парсинг логинов
        auto login1 = fname.substr(0, pos);
        auto login2 = fname.substr(pos + 1);
        // получаем пользователей
        auto u1 = search_User(login1);
        auto u2 = search_User(login2);
        // пусто - пропускаем
        if (!u1 || !u2) continue;
        
        // запаковываем логины и имена наших собеседников
        if (u1->getLogin() == my_login)
        {
            send.push_back({u2->getLogin(), u2->getName()});
        }
        else if (u2->getLogin() == my_login)
        {
            send.push_back({u1->getLogin(), u2->getName()});
        }
    }
    return send;
}


/*=====================================
        ФУНКЦИИ ЧАТОВ
=====================================*/

//вспомогательная функция собирает название чата из предосталенных логинов
std::string DataBaseFile::fileNameChatP(std::string user1, std::string user2){
    std::string fileChat = "file/";
    fileChat += user1;
    fileChat += "_";
    fileChat += user2;
    fileChat += ".txt";
    return fileChat;
}

//запись в приватный чат (Отправитель, получатель, сообщение)
bool DataBaseFile::write_Chat_P(std::shared_ptr<User> user_sender, std::shared_ptr<User> user_recipient, const std::string& mess) {
    
    // собираем название чата
    std::string fileName;
    //Если существует такой файл
    if (fileExists(fileNameChatP(user_sender->getLogin(), user_recipient->getLogin()))) {
        fileName = fileNameChatP(user_sender->getLogin(), user_recipient->getLogin());
    } else if (fileExists(fileNameChatP(user_recipient->getLogin(), user_sender->getLogin()))){
        fileName = fileNameChatP(user_recipient->getLogin(), user_sender->getLogin());
    } else {
        //Нету такого файла создаем заного
        fileName = fileNameChatP(user_sender->getLogin(), user_recipient->getLogin());
        createFile(fileName);
    }
    
    std::ofstream fs(fileName, std::ios::app);
    if (fs.is_open()) {
        fs << user_sender->getLogin() << '\n'
           << mess << "\n\n";
        fs.close();
        return true;
    } else {
        std::cerr << "Ошибка записи в файл чата: " << fileName << std::endl;
        return false;
    }
}


// Загрузить историю приватного чата: пары <login, сообщение>
bool DataBaseFile::load_Chat_P(std::shared_ptr<User> user_sender, std::shared_ptr<User> user_recipient, std::vector<std::pair<std::string, std::string>>& out) {

    // собираем название чата
    std::string fileName;
    //Если существует такой файл
    if (fileExists(fileNameChatP(user_sender->getLogin(), user_recipient->getLogin()))) {
        fileName = fileNameChatP(user_sender->getLogin(), user_recipient->getLogin());
    } else if (fileExists(fileNameChatP(user_recipient->getLogin(), user_sender->getLogin()))){
        fileName = fileNameChatP(user_recipient->getLogin(), user_sender->getLogin());
    } else {
        //Нету такого файла
        return false;
    }

    std::ifstream fs(fileName);

    if (!fs.is_open()) return false;

    std::string login, mess;
    while (std::getline(fs, login) && std::getline(fs, mess)) {
        out.emplace_back(login, mess);
        //пропуск разделитяля
        std::string sep;
        std::getline(fs, sep);
    }
    return true;
}


//запись в общий чат, проверить перед записью существоваие файла!
bool DataBaseFile::write_Chat_H(std::shared_ptr<User> user_sender, const std::string& mess) {
    
    createDirectoryIfNeeded(CHAT_HARED_FILE);
    //Если существует такой файл
    if (!fileExists(CHAT_HARED_FILE)) {
        //Нету такого файла создаем заного
        createFile(CHAT_HARED_FILE);
    }
    
    std::ofstream fs(CHAT_HARED_FILE, std::ios::app);
    if (fs.is_open()) {
        fs << user_sender->getLogin() << '\n'
           << user_sender->getName() << '\n'
           << mess << "\n\n";
        fs.close();
        return true;
    } else {
        std::cerr << "Ошибка записи в файл чата: " << CHAT_HARED_FILE << std::endl;
        return false;
    }
}


// Загрузить историю общего чата: пары <login, name, сообщение>
bool DataBaseFile::load_Chat_H(std::vector<std::vector<std::string>>& out) {

    createDirectoryIfNeeded(CHAT_HARED_FILE);
    //Если существует такой файл
    if (!fileExists(CHAT_HARED_FILE)) {
        //Нету такого файла создаем заного
        createFile(CHAT_HARED_FILE);
    }

    std::ifstream fs(CHAT_HARED_FILE);

    if (!fs.is_open()) return false;

    std::string login, name, mess;
    while (std::getline(fs, login) && std::getline(fs, name) && std::getline(fs, mess)) {
        out.push_back({login, name, mess});
        //пропуск разделитяля
        std::string sep;
        std::getline(fs, sep);
    }
    return true;
}
