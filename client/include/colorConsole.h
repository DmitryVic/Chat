#pragma once


// Цветовые макросы
#define _RED     "\x1B[31m"             // Красный
#define _GREEN   "\x1B[32m"             // Зеленый
#define _YELLOW  "\x1B[33m"             // Желтый
#define _BLUE    "\x1B[34m"             // Голубой
#define _MAGENTA "\x1B[35m"             // Фиолетовый
#define _CYAN    "\x1B[36m"             // Берюзовый
#define _CLEAR   "\033[0m"              // Очистить - Белый

#define _GREY_BG "\033[3;100;30m"       // Серый фон

/*

Пример использования:

std::cout << _RED <<  "Texting" <<  _CLEAR << "\t\t";
printf(_GREEN "Texting" _CLEAR "\t\t");

*/