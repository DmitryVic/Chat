


add_executable(chat_client
    src/console_interface.cpp 
    src/interaction_chat.cpp   
    src/main.cpp
    src/MessageHandler.cpp
    src/NetworkClient.cpp
    src/UserStatus.cpp
)

# линкует клиентский исполняемый файл с общей библиотекой
target_link_libraries(chat_client PRIVATE common)
# общая библиотека нужна только для сборки PRIVATE
target_include_directories(chat_client PRIVATE include)

#Добавляем пути к  заголовкам клиента
target_include_directories(chat_client PRIVATE 
    include
)