#pragma once
#include <string>

struct dataUserOnline
{
    std::string online_user_login;
    int client_socket;
};

// У каждого потока своя копия
extern thread_local dataUserOnline currentUser;