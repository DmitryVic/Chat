#pragma once
#include "BD.h"
#include "NetworkServer.h"
#include <memory>
#include <string>
#include "BD_MySQL.h"


void chat_start(std::shared_ptr<NetworkServer> network);