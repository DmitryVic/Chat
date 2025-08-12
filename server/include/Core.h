#pragma once
#include "BD.h"
#include "NetworkServer.h"
#include <memory>
#include <string>

 //Для авторизации пользователя
extern std::string online_user_login;

void chat_start(std::shared_ptr<DataBase> db, std::shared_ptr<NetworkServer> network);