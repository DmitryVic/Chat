

# компиляция только для Linux
if(UNIX)
    add_executable(chat_server
        src/main.cpp
        src/Core.cpp
        src/BD.cpp
        #src/Network.cpp
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