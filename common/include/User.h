#pragma once
#include <string>
#include <vector>



class User
{
private:
    std::string _login;                                  // Уникален Максимум 10 знаков анлгл, запретить другие
    std::string _pasword;                               //храниться хэш вместо пароля
    std::string _name;
    std::vector<unsigned int> _connectChatPrivate;       // Хранит уникальные ID чатов 
public:
    // Передать логин, пароль, имя
    User(const std::string& login, const std::string& pasword, const std::string& name)
    : _login(login), _pasword(pasword), _name(name) {}
    
    ~User() {}
    
    // Получить логин
    std::string getLogin() const;

    // Получить имя
    std::string getName() const;

    // Получить проль не безопасный
    std::string getPass() const;

    // Получить список чатов пользователя
    std::vector<int> getIDConnectionChatPrivate() const;

    // Добавить пользователя в приватный чат
    // вернет true - при успехе, false - при ошибке
    bool setChat(unsigned int id_Chat);

    // проверяет есть чаты с данным узером
    bool userInChatsP(unsigned int id_Chat) const;

};


