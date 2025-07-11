#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <string>



// Файл существует или нет
bool fileExists(const std::string& filename);


// Создает директорию, если её нет
void createDirectoryIfNeeded(const std::string& path);


// Создать файл
void createFile(const std::string& filename);

void log_write(std::string& strr);