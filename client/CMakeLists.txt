
if(WIN32)
    set(NETWORK_CLIENT_SRC src/NetworkClientWin.cpp)
else()
    set(NETWORK_CLIENT_SRC src/NetworkClientLinux.cpp)
endif()

add_executable(chat_client
    src/console_interface.cpp 
    src/interaction_chat.cpp   
    src/main.cpp
    src/MessageHandler.cpp
    ${NETWORK_CLIENT_SRC}
    src/UserStatus.cpp
)

# Для Windows-клиента
if(WIN32)
    target_link_libraries(chat_client PRIVATE ws2_32)
endif()

# Для Linux-клиента
if(UNIX AND NOT APPLE)
    target_compile_definitions(chat_client PRIVATE SET_GLOBAL_LOCALE_LINUX)
endif()

# линкует клиентский исполняемый файл с общей библиотекой
target_link_libraries(chat_client PRIVATE common)
# общая библиотека нужна только для сборки PRIVATE
target_include_directories(chat_client PRIVATE include)

#Добавляем пути к  заголовкам клиента
target_include_directories(chat_client PRIVATE 
    include
)