
# СПринудительно установить стандарт c++ 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# компиляция только для Linux
if(UNIX)
    add_executable(chat_server
        src/main.cpp
        src/Core.cpp
        src/BD.cpp
        src/NetworkServer.cpp
        src/MessageHandler.cpp
    )

    # линковка с общей библиотекой
    target_link_libraries(chat_server PRIVATE common)

    #Добавляем пути к  заголовкам сервера
    target_include_directories(chat_server PRIVATE 
        include
    )

else()
    message(WARNING "Компиляция без серверной части, так как она только для Linux")
endif()