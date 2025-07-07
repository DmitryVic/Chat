#include "User.h"


// Получить логин
std::string User::getLogin() const{
    return this->_login;
}


// Получить имя
std::string User::getName() const{
    return this->_name;
}


// Получить проль не безопасный
std::string User::getPass() const{
    return this->_pasword;
}
