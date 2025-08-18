#pragma once
#include "BD.h"
#include "NetworkServer.h"
#include <memory>
#include <string>


void chat_start(std::shared_ptr<DataBase> db, std::shared_ptr<NetworkServer> network);