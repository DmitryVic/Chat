#include "BD.h"
#include "BD_MySQL.h"
#include "User.h"
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <mysql/mysql.h>

namespace fs = std::filesystem;



DataBaseMySQL::DataBaseMySQL()
{
    mysql_init(&mysql);

    // Подключение
    if (!mysql_real_connect(&mysql, "localhost", this->USER, this->PASS, this->BD, 0, NULL, 0)) {
        std::cerr << "Ошибка подключения БД (MySQL): " << mysql_error(&mysql) << std::endl;
        throw "Ошибка подключения БД (MySQL) - исключение конструктора";
    }
    std::cerr << "Подключение БД (MySQL) успешно!" << std::endl;

    // Кодировка
    mysql_set_character_set(&mysql, "utf8");
    std::cerr << "Кодировка БД (MySQL): " << mysql_character_set_name(&mysql) << std::endl;

    // Создание таблицы users
    if (mysql_query(&mysql, "CREATE TABLE IF NOT EXISTS users(id INT AUTO_INCREMENT PRIMARY KEY, login VARCHAR(30) UNIQUE NOT NULL, name VARCHAR(30), surname VARCHAR(30), email VARCHAR(50) UNIQUE);") != 0) {
        throw "Ошибка БД (MySQL) оздания таблицы users: " + std::string(mysql_error(&mysql));
    }
    // Создание таблицы user_passwords
    if (mysql_query(&mysql, "CREATE TABLE IF NOT EXISTS user_passwords(user_id INT NOT NULL, PRIMARY KEY (user_id), FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE, pass VARCHAR(255) NOT NULL);") != 0) {
        throw "Ошибка БД (MySQL) оздания таблицы user_passwords: " + std::string(mysql_error(&mysql));
    }

    // Создание таблицы chats
    if (mysql_query(&mysql, "CREATE TABLE IF NOT EXISTS chats(id INT AUTO_INCREMENT PRIMARY KEY, type ENUM('private','common') NOT NULL);") != 0) {
        throw "Ошибка БД (MySQL) оздания таблицы chats: " + std::string(mysql_error(&mysql));
    }

    // Создание таблицы Связи пользователей с чатами user_chats
    if (mysql_query(&mysql, "CREATE TABLE IF NOT EXISTS user_chats(user_id INT NOT NULL, chat_id INT NOT NULL, FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE, FOREIGN KEY (chat_id) REFERENCES chats(id) ON DELETE CASCADE, PRIMARY KEY (user_id, chat_id));") != 0) {
        throw "Ошибка БД (MySQL) оздания таблицы user_chats: " + std::string(mysql_error(&mysql));
    }

    // Создание таблицы messages
    if (mysql_query(&mysql, "CREATE TABLE IF NOT EXISTS messages(id INT AUTO_INCREMENT PRIMARY KEY, chat_id INT NOT NULL, FOREIGN KEY (chat_id) REFERENCES chats(id) ON DELETE CASCADE, sender_id INT NOT NULL, FOREIGN KEY (sender_id) REFERENCES users(id) ON DELETE CASCADE, created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP, text TEXT NOT NULL, recipient_id INT NULL,  FOREIGN KEY (recipient_id) REFERENCES users(id) ON DELETE SET NULL);") != 0) {
        throw "Ошибка БД (MySQL) оздания таблицы messages: " + std::string(mysql_error(&mysql));
    }

    //создание чата общего если его нет
    std::string request_mysql =  "SELECT id FROM chats WHERE type = 'common';";
    if (mysql_query(&mysql, request_mysql.c_str()) != 0) {
        std::cerr << "Ошибка БД (MySQL) не прошел запрос получения информации общего чата при инициализации БД: " << std::string(mysql_error(&mysql)) << std::endl;
        throw "Ошибка БД (MySQL) не прошел запрос получения информации общего чата при инициализации БД: " + std::string(mysql_error(&mysql));
    }

    // Получение результата
    res = mysql_store_result(&mysql);
    
    if (!res) { // Проверка на NULL результата
        if (mysql_errno(&mysql)) {
            // Ошибка выполнения запроса
            throw std::runtime_error(mysql_error(&mysql));
        }
        // Нет результатов создаем общий чат
        mysql_free_result(res);
        request_mysql =  "INSERT INTO chats (type) VALUES ('common');";
        if (mysql_query(&mysql, request_mysql.c_str()) != 0) {
            std::cerr << "Ошибка БД (MySQL) создания общего чата: " << std::string(mysql_error(&mysql)) << std::endl;
            throw "Ошибка БД (MySQL) создания общего чата: " + std::string(mysql_error(&mysql));
        }
    }
    else{
        row = mysql_fetch_row(res);
        if (!row) { // Нет подходящих записей ДОП ПРОВЕРКА
            mysql_free_result(res);
            request_mysql =  "INSERT INTO chats (type) VALUES ('common');";
            if (mysql_query(&mysql, request_mysql.c_str()) != 0) {
                std::cerr << "Ошибка БД (MySQL) создания общего чата: " << std::string(mysql_error(&mysql)) << std::endl;
                throw "Ошибка БД (MySQL) создания общего чата: " + std::string(mysql_error(&mysql));
            }
        }
        else
        {
            // Проверка количества полей
            unsigned num_fields = mysql_num_fields(res);
            if (num_fields != 1) {
                mysql_free_result(res);
                throw std::runtime_error("Ожидалось 1 поле в результате получения информации общего чата при инициализации БД");
            }
            // НЕТ ПРОВЕРКИ НА НЕСКОЛЬКО ПОДОБНЫХ ЧАТОВ В БД
        }
    }

    


    

}

DataBaseMySQL::~DataBaseMySQL(){
    // Закрытие соединения
    mysql_close(&mysql);
}


//=================================================================
// БД: запись и чтение структур
//=================================================================


//запись юзера
void DataBaseMySQL::write_User(std::shared_ptr<User> user) {
    // запись в таблицу users
    std::string request_mysql =  
    "INSERT INTO users (login, name) VALUES (\"" 
    + user->getLogin()
    + "\", \""
    + user->getName()
    + "\");";

    if (mysql_query(&mysql, request_mysql.c_str()) != 0) {
        std::cerr << "Ошибка БД (MySQL) запись в таблицу users: " << std::string(mysql_error(&mysql)) << std::endl;
        throw "Ошибка БД (MySQL) запись в таблицу users: " + std::string(mysql_error(&mysql));
    }

    // запись в таблицу user_passwords
    request_mysql =  
    "INSERT INTO user_passwords (user_id, pass) SELECT id, \"" 
    + user->getPass()
    + "\" FROM users WHERE login = \""
    + user->getLogin()
    + "\";";

    if (mysql_query(&mysql, request_mysql.c_str()) != 0) {
        std::cerr << "Ошибка БД (MySQL) запись в таблицу user_passwords: " << std::string(mysql_error(&mysql)) << std::endl;
        throw "Ошибка БД (MySQL) запись в таблицу user_passwords: " + std::string(mysql_error(&mysql));
    }
}


//Поиск юзера пологину
std::shared_ptr<User> DataBaseMySQL::search_User(const std::string& log){
    std::string request_mysql =  "SELECT u.login, up.pass, u.name FROM users u JOIN user_passwords up ON u.id = up.user_id WHERE u.login = '"
    + log + "';";

    // Выполнение запроса
    if (mysql_query(&mysql, request_mysql.c_str()) != 0) {
        std::cerr << "Ошибка поиска пользователя в БД (MySQL): " << mysql_error(&mysql) << std::endl;
        throw "Ошибка поиска пользователя в БД (MySQL): " + std::string(mysql_error(&mysql));
    }

    // Получение результата
    res = mysql_store_result(&mysql);
    
    if (!res) { // Проверка на NULL результата
        if (mysql_errno(&mysql)) {
            // Ошибка выполнения запроса
            throw std::runtime_error(mysql_error(&mysql));
        }
        // Нет результатов
        return nullptr;
    }

    row = mysql_fetch_row(res);
    if (!row) { // Нет подходящих записей
        mysql_free_result(res);
        return nullptr;
    }


    // Проверка количества полей
    unsigned num_fields = mysql_num_fields(res);
    if (num_fields != 3) {
        mysql_free_result(res);
        throw std::runtime_error("Ожидалось 3 поля в результате поиска пользователя в БД (MySQL)");
    }

    // Безопасное преобразование
    std::string login = row[0] ? row[0] : "";
    std::string pass = row[1] ? row[1] : "";
    std::string name = row[2] ? row[2] : "";

    mysql_free_result(res);
    return std::make_shared<User>(login, pass, name);
}


// Получить список всех юзеров исключая юзера, который делает запрос
std::vector<std::pair<std::string, std::string>> DataBaseMySQL::list_all_User(std::string my_login){
    
    std::vector<std::pair<std::string, std::string>> send;
    std::string request_mysql =  "SELECT u.login, u.name FROM users u WHERE u.login <> '" + my_login + "';";
    
    // Выполнение запроса
    if (mysql_query(&mysql, request_mysql.c_str()) != 0) {
        std::cerr << "Ошибка БД (MySQL) запроса выдачи всех пользователей: " << mysql_error(&mysql) << std::endl;
        throw "Ошибка БД (MySQL) запроса выдачи всех пользователей: " + std::string(mysql_error(&mysql));
    }

    // Получение результата
    res = mysql_store_result(&mysql);
    
    if (!res) { // Проверка на NULL результата
        if (mysql_errno(&mysql)) {
            // Ошибка выполнения запроса
            throw std::runtime_error(mysql_error(&mysql));
        }
        // Нет результатов
        return send;
    }

    row = mysql_fetch_row(res);
    if (!row) { // Нет подходящих записей
        mysql_free_result(res);
        return send;
    }


    // Количество полей
    unsigned num_fields = mysql_num_fields(res);
    if (num_fields != 2) {
        mysql_free_result(res);
        throw std::runtime_error("БД (MySQL) Ожидалось 2 поля в результате запроса выдачи всех пользователей");
    }

    while ((row = mysql_fetch_row(res))) {
        send.push_back({row[0], row[1]});
    }

    mysql_free_result(res);
    return send;
}


//получение бзеров с кем есть приватный чат, возвращает логины и имена для отображения
// std::vector<std::pair<std::string ЛОГИН, std::string ИМЯ>>
std::vector<std::pair<std::string, std::string>> DataBaseMySQL::my_chat_P(std::string my_login) {
    std::vector<std::pair<std::string, std::string>> send;
    
    return send;
}


/*=====================================
        ФУНКЦИИ ЧАТОВ
=====================================*/


//запись в приватный чат (Отправитель, получатель, сообщение)
bool DataBaseMySQL::write_Chat_P(std::shared_ptr<User> user_sender, std::shared_ptr<User> user_recipient, const std::string& mess) {
    
   



    return false;

}


// Загрузить историю приватного чата: пары <login, сообщение>
bool DataBaseMySQL::load_Chat_P(std::shared_ptr<User> user_sender, std::shared_ptr<User> user_recipient, std::vector<std::pair<std::string, std::string>>& out) {

    
    
    return true;
}


//запись в общий чат, проверить перед записью существоваие файла!
bool DataBaseMySQL::write_Chat_H(std::shared_ptr<User> user_sender, const std::string& mess) {
    
    // ПРАВИЛЬНЫЙ ЗАПРОС НА ДОБАВЛЕНИЕ ДАННЫХ БЕЗ ПРОВЕРКИ АЙДИ А СРАЗУ В ЗАПРОСЕ ПРОВЕРЯЕМ  


    // INSERT INTO messages (chat_id, sender_id, text)
    // VALUES (
    // (SELECT min(id) FROM chats WHERE type = 'common' ORDER BY id LIMIT 1),
    // (SELECT u.id FROM users u WHERE u.login = 'login1' LIMIT 1),
    // 'text'
    // );
    
    
    // получаем приватный чат ORDER BY id для надежности взять верхний (минимальный) а в методе отображения минимальный отображается
    std::string request_mysql =  "SELECT id FROM chats WHERE type = 'common' ORDER BY id;";
    // Выполнение запроса
    if (mysql_query(&mysql, request_mysql.c_str()) != 0) {
        std::cerr << "Ошибка БД (MySQL) получения id приватного чата в write_Chat_P: " << mysql_error(&mysql) << std::endl;
        return false;
    }
     // Получение результата
    res = mysql_store_result(&mysql);
    
    if (!res) { // Проверка на NULL результата
        if (mysql_errno(&mysql)) {
            // Ошибка выполнения запроса
            std::cerr << "Ошибка БД (MySQL) приватный чат, метод - write_Chat_P: " << mysql_error(&mysql) << std::endl;
            return false;
        }
        // Нет результатов
        std::cerr << "Ошибка БД (MySQL) приватный чат отсутствует, метод - write_Chat_P" << std::endl;
        return false;
    }

    row = mysql_fetch_row(res);
    if (!row) { // Нет записей
        mysql_free_result(res);
        std::cerr << "Ошибка БД (MySQL) нет записей row, метод - write_Chat_P" << std::endl;
        return false;
    }

    // Количество полей
    unsigned num_fields = mysql_num_fields(res);
    if (num_fields != 1) {
        mysql_free_result(res);
        std::cerr << "БД (MySQL) Ожидалось 1 поле в результате получения id приватного чата в write_Chat_P" << std::endl;
        return false;
    }
    std::string id_chat_P = row[0];
    mysql_free_result(res);
    
    
    return false;

}


// Загрузить историю общего чата: пары <login, name, сообщение>
bool DataBaseMySQL::load_Chat_H(std::vector<std::vector<std::string>>& out) {
    // SELECT u.name, m.text
    // FROM messages m
    // JOIN users u ON u.id = m.sender_id
    // WHERE m.chat_id IN (SELECT min(id) as id_chat_p FROM chats WHERE type = 'common');
    return true;
}
