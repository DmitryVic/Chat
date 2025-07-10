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


//запись юзера в БД
void BD_write_User(std::shared_ptr<User> user);


//Поиск юзера пологину
std::shared_ptr<User> BD_search_User(const std::string& log);

//получение бзеров с кем есть приватный чат, возвращает логины и имена для отображения
// std::vector<std::pair<std::string ЛОГИН, std::string ИМЯ>>
std::vector<std::pair<std::string, std::string>> my_chat_P(std::string my_login);

// Получить список всех юзеров
std::vector<std::pair<std::string, std::string>> list_all_User(std::string my_login);


/*=====================================
        ФУНКЦИИ ЧАТОВ
=====================================*/


//запись в приватный чат (Отправитель, получатель, сообщение)
bool write_Chat_P(std::shared_ptr<User> user_sender, std::shared_ptr<User> user_recipient, const std::string& mess);


// Загрузить историю приватного чата: пары <login, сообщение>
bool load_Chat_P(std::shared_ptr<User> user_sender, std::shared_ptr<User> user_recipient, std::vector<std::pair<std::string, std::string>>& out);


//запись в общий чат, проверить перед записью существоваие файла!
bool write_Chat_H(std::shared_ptr<User> user_sender, const std::string& mess);


// Загрузить историю общего чата: пары <login, сообщение>
bool load_Chat_H(std::vector<std::pair<std::string, std::string>>& out);

