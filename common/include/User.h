#pragma once
#include <string>
#include <vector>


class User
{
private:
    std::string _login;                                  // Уникален Максимум 10 знаков анлгл, запретить другие
    std::string _pasword;                               //храниться хэш вместо пароля
    std::string _name;

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

};


