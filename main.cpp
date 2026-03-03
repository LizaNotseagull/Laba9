#include <iostream>
#include <clocale>
#include <limits>
#include "Header.h"

int main() {
    std::setlocale(LC_ALL, "RU");
    int V;
    do {
        std::cout << "Выберите: " << std::endl << std::endl;
        std::cout << "1 - Задача 1 ( положительные/отрицательные числа в обратном порядке)" << std::endl;
        std::cout << "2 - Задача 2 (Удвоить размер файла)" << std::endl;
        std::cout << "3 - Задача 3 (Объединить несколько файлов в новом файле-архиве )" << std::endl;
        std::cout << "4 - Задача 4 (Вычислить выражение)" << std::endl;
        std::cout << "3 - Задача 5 (Чтение бинарных файлов)" << std::endl;
        std::cout << "4 - Задача 6 (Создание бинарных файлов)" << std::endl;
        std::cout << "0 - Завершить ?" << std::endl << std::endl;
        std::cout << "Выберите задачу: ";
        std::cin >> V;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // очистка буфера

        switch (V) {
        case 1:
            N_1();
            break;
        case 2:
            N_2();
            break;
        case 3:
            N_3();
            break;
        case 4:
            N_4();
            break;
        case 5:
            N_5();
            break;
        case 6:
            N_6();
            break;
        case 0:
            std::cout << "Завершаю" << std::endl;
            break;
        default:
            std::cout << "Выберите от 0 до 6" << std::endl;
        }
    } while (V != 0);
    return 0;
}