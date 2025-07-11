#pragma once
#include "BD.h"
#include "Network.h"
#include <memory>


void chat_start(std::shared_ptr<DataBase> db, std::shared_ptr<NetworkServer> network);