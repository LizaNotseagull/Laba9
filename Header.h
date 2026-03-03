#pragma once
#ifndef FUNC
#define FUNC
#include <string>

void N_1();
void N_2();
void N_3();
void N_4();
void N_5();
void N_6();

// Функция: читает целые числа из текстового файла и записывает их в бинарный файл
bool txt_to_bin(const std::string& txt_file, const std::string& bin_file, int& total_numbers);

// Функция: обрабатывает бинарный файл (читает с конца) и записывает положительные и отрицательные числа
bool process_bin(const std::string& src_bin, const std::string& pos_bin, const std::string& neg_bin, int& pos_count, int& neg_count);

// Функция выводит содержимое бинарного файла на экран (для проверки)
void print_bin_file(const std::string& filename);

// Функция удваивает содержимое бинарного файла
bool double_binary_file(const std::string& filename);

// Функция возвращает количество целых чисел в бинарном файле
long long get_file_size(const std::string& filename);

// Дополнительная функция для чтения и вывода бинарного файла
void printBinaryFile(const std::string& filename);

#endif