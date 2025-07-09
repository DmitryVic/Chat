#pragma once
#include <string>


// Обработка входящих сообщений
void handle_incoming_message(const std::string& json_str, int client_sock);