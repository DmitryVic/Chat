#pragma once
#include "User.h" 
#include <string>
#include <vector>
#include <utility>
#include <memory>


/*=====================================
        ОБЩИЕ ФУНКЦИИ
=====================================*/

//Файл существует или нет
bool fileExists(const std::string& filename);

// Создает директорию, если её нет
void createDirectoryIfNeeded(const std::string& path);

// Создать файл
void createFile(const std::string& filename);

// Получить список всех файлов чатов в директории
std::vector<std::string> listChatFiles(const std::string& dir);

/*=====================================
        ФУНКЦИИ БД
=====================================*/

//запись юзера
void BD_write_User(std::shared_ptr<User> user);

//Поиск юзера пологину
std::shared_ptr<User> BD_search_User(const std::string& log);

/*=====================================
        ФУНКЦИИ ЧАТОВ
=====================================*/

//запись в приватный чат, проверить перед записью существоваие файла!
void write_Chat_P(const std::string& filename, std::shared_ptr<User> user, const std::string& mess);


// Загрузить историю приватного чата: пары <login, сообщение>
bool load_Chat_P(const std::string& filename,
std::vector<std::pair<std::string, std::string>>& out);
