#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;



std::string FILE_LOG = "file/log.txt";



// Файл существует или нет
bool fileExists(const std::string& filename) {
    std::ifstream file(filename);
    return file.good();
}


// Создает директорию, если её нет
void createDirectoryIfNeeded(const std::string& path) {

    auto dir_path = fs::path(path).parent_path();
    if (!dir_path.empty() && !fs::exists(dir_path)) {
        fs::create_directories(dir_path);
    }

}


// Создать файл
void createFile(const std::string& filename) {
    createDirectoryIfNeeded(filename);

    std::ofstream file(filename, std::ios::out);

    if (file.is_open()) {
        file.close();
    } else {
        std::cerr << "Не удалось создать файл " << filename << std::endl;
    }
}

void log_write(std::string strr){
    createDirectoryIfNeeded(FILE_LOG);   
    std::ofstream fs(FILE_LOG, std::ios::app);
    if (fs.is_open()) {
        fs << strr << '\n';
        fs.close();
    } else {
        std::cerr << "Ошибка записи в файл логов: " << FILE_LOG << std::endl;
    }
}

void log_write(const std::exception& e) {
    log_write(std::string("Ошибка: ") + e.what());
}