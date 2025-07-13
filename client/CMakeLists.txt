


add_executable(chat_client
    src/main.cpp
    src/NetworkClient.cpp
)

# линкует клиентский исполняемый файл с общей библиотекой
target_link_libraries(chat_client PRIVATE common)
# общая библиотека нужна только для сборки PRIVATE
target_include_directories(chat_client PRIVATE include)

#Добавляем пути к  заголовкам клиента
target_include_directories(chat_client PRIVATE 
    include
)