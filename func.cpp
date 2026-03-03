#include <iostream>
#include <fstream>
#include <string>
#include <cctype>   // для isdigit
#include <cstdlib> 
#include "Header.h"

// Задача 1

// Читает целые числа из текстового файла и записывает их в бинарный файл.
bool txt_to_bin(const std::string& txt_file, const std::string& bin_file, int& total_numbers) {
    // Открываем текстовый файл для чтения
    std::ifstream ftxt(txt_file);
    if (!ftxt) {   // если не удалось открыть
        std::cerr << "Ошибка открытия текстового файла " << txt_file << std::endl;
        return false;
    }

    // Создаём бинарный файл для записи
    std::ofstream fbin(bin_file, std::ios::binary);
    if (!fbin) {
        std::cerr << "Ошибка создания бинарного файла " << bin_file << std::endl;
        return false;
    }

    int num;
    total_numbers = 0;
    // Читаем числа из текстового файла
    while (ftxt >> num) {
        // Записываем число в бинарном виде
        fbin.write((char*)&num, sizeof(num));
        ++total_numbers;
    }

    ftxt.close();
    fbin.close();

    if (total_numbers == 0) {
        std::cout << "В текстовом файле нет чисел. Будет создан пустой бинарный файл.\n";
    }
    return true;
}

// Читает бинарный файл с конца, разделяет числа на положительные и отрицательные и записывает их в соответствующие файлы в обратном порядке.
bool process_bin(const std::string& src_bin, const std::string& pos_bin, const std::string& neg_bin, int& pos_count, int& neg_count) {
    // Открываем исходный бинарный файл для чтения
    std::ifstream fsrc(src_bin, std::ios::binary);
    if (!fsrc) {
        std::cerr << "Ошибка открытия исходного бинарного файла " << src_bin << std::endl;
        return false;
    }

    // Определяем размер файла и количество чисел
    fsrc.seekg(0, std::ios::end);          // перемещаем указатель в конец
    std::streampos file_size = fsrc.tellg(); // получаем размер в байтах
    long long total = file_size / sizeof(int); // количество чисел (каждое занимает 4 байта)

    if (total == 0) {
        std::cout << "Бинарный файл пуст. Выходные файлы будут созданы пустыми.\n";
        fsrc.close();
        // Создаём пустые выходные файлы, чтобы они существовали
        std::ofstream fpos(pos_bin, std::ios::binary);
        std::ofstream fneg(neg_bin, std::ios::binary);
        fpos.close();
        fneg.close();
        pos_count = neg_count = 0;
        return true;
    }

    // Создаём выходные бинарные файлы для положительных и отрицательных чисел
    std::ofstream fpos(pos_bin, std::ios::binary);
    std::ofstream fneg(neg_bin, std::ios::binary);
    if (!fpos || !fneg) {
        std::cerr << "Ошибка создания выходных файлов " << pos_bin << " или " << neg_bin << std::endl;
        fsrc.close();
        return false;
    }

    pos_count = neg_count = 0;
    int num;

    // Читаем числа с конца файла, чтобы записать их в обратном порядке
    for (long long i = total - 1; i >= 0; --i) {
        // Перемещаем указатель на начало i-го числа (смещение i * sizeof(int) от начала)
        fsrc.seekg(i * sizeof(int), std::ios::beg);
        // Читаем число в переменную num
        fsrc.read((char*)&num, sizeof(num));

        // Проверяем, успешно ли прошло чтение (поток не в состоянии ошибки)
        if (!fsrc) {
            std::cerr << "Ошибка чтения числа из бинарного файла (индекс " << i << ")\n";
            break;
        }

        // Классифицируем число и записываем в соответствующий файл
        if (num > 0) {
            fpos.write((char*)&num, sizeof(num));
            ++pos_count;
        }
        else if (num < 0) {
            fneg.write((char*)&num, sizeof(num));
            ++neg_count;
        }
        // нули игнорируются
    }

    fsrc.close();
    fpos.close();
    fneg.close();
    return true;
}

// Выводит содержимое бинарного файла на экран (для проверки).
void print_bin_file(const std::string& filename) {
    std::ifstream f(filename, std::ios::binary);
    if (!f) {
        std::cout << "Не удалось открыть " << filename << " для чтения.\n";
        return;
    }
    int num;
    std::cout << "Содержимое " << filename << ": ";
    bool empty = true;
    // Читаем числа одно за другим до конца файла
    while (f.read((char*)&num, sizeof(num))) {
        std::cout << num << " ";
        empty = false;
    }
    if (empty) std::cout << "(файл пуст)";
    std::cout << std::endl;
    f.close();
}

void N_1() {
    std::setlocale(LC_ALL, "ru"); 

    // Имена файлов по умолчанию
    std::string txt_input = "numbers.txt";
    std::string temp_bin = "temp.bin";
    std::string pos_bin = "positive.bin";
    std::string neg_bin = "negative.bin";

    // Запрашиваем имя входного текстового файла
    std::cout << "Введите имя текстового файла с числами (по умолчанию numbers.txt): ";
    std::string filename;
    std::getline(std::cin, filename);
    if (filename.empty()) filename = txt_input;   // если Enter – используем по умолчанию

    // Конвертация текстового файла в бинарный
    int total;
    if (!txt_to_bin(filename, temp_bin, total)) {
        std::exit(1);   // при ошибке завершаем программу
    }
    std::cout << "Прочитано чисел из текстового файла: " << total << std::endl;

    //Обработка бинарного файла и запись положительных/отрицательных
    int pos_count, neg_count;
    if (!process_bin(temp_bin, pos_bin, neg_bin, pos_count, neg_count)) {
        std::exit(1);
    }

    //  Удаляем временный бинарный файл
    std::remove(temp_bin.c_str());


    std::cout << "\nРезультаты:\n";
    std::cout << "Положительных чисел (в обратном порядке) записано в " << pos_bin << ": " << pos_count << std::endl;
    std::cout << "Отрицательных чисел (в обратном порядке) записано в " << neg_bin << ": " << neg_count << std::endl;

    std::cout << "\nПроверка:\n";
    print_bin_file(pos_bin);
    print_bin_file(neg_bin);
}
// Задача 2 


// Функция удваивает содержимое бинарного файла, дописывая его копию в конец.
bool double_binary_file(const std::string& filename) {
    //Открываем файл для чтения
    std::ifstream src(filename, std::ios::binary);  // создаём входной поток
    if (!src) {                                      // проверяем, открылся ли файл
        std::cerr << "Ошибка открытия файла " << filename << " для чтения.\n";
        return false;
    }

    // Определяем размер файла и количество чисел
    src.seekg(0, std::ios::end);        // перемещаем указатель чтения в конец
    std::streampos size = src.tellg();  // получаем позицию = размер в байтах
    long long count = size / sizeof(int); // количество целых чисел (размер / 4)
    src.seekg(0, std::ios::beg);        // возвращаемся в начало файла

    // Если файл пуст, ничего не делаем
    if (count == 0) {
        std::cout << "Файл пуст. Нечего удваивать.\n";
        src.close();
        return true; 
    }

    // Открываем тот же файл для добавления в конец
    // Флаг ios::app обеспечивает запись всегда в конец, не удаляя существующие данные.
    std::ofstream dest(filename, std::ios::binary | std::ios::app);
    if (!dest) {
        std::cerr << "Ошибка открытия файла " << filename << " для добавления.\n";
        src.close();
        return false;
    }

    // Последовательно читаем числа из начала и дописываем их в конец
    int num;                           // переменная для хранения прочитанного числа
    for (long long i = 0; i < count; ++i) {
        // Читаем очередное число из исходного файла
        src.read(reinterpret_cast<char*>(&num), sizeof(num));
        if (!src) {                    
            std::cerr << "Ошибка чтения числа\n";
            src.close();
            dest.close();
            return false;
        }

        // Записываем это же число в конец файла
        dest.write(reinterpret_cast<char*>(&num), sizeof(num));
        if (!dest) {                    
            std::cerr << "Ошибка записи числа\n";
            src.close();
            dest.close();
            return false;
        }
    }

    //Закрываем файлы и выводим результат
    src.close();
    dest.close();
    std::cout << "Файл успешно удвоен. Было " << count << " чисел, стало " << (count * 2) << ".\n";
    return true;
}

void N_2() {
    std::setlocale(LC_ALL, "ru");   


    std::cout << "Введите имя бинарного файла (по умолчанию positive.bin): ";
    std::string filename;
    std::getline(std::cin, filename);
    if (filename.empty()) filename = "positive.bin";

    //Показываем содержимое до удвоения
    std::cout << "\nДо операции:\n";
    print_bin_file(filename);   // функция вывода бинарного файла

    //Выполняем удвоение
    if (!double_binary_file(filename)) {
        std::exit(1);
    }

    std::cout << "\nПосле операции:\n";
    print_bin_file(filename);
}
// Задача 3

// Функция возвращает количество целых чисел в бинарном файле.
long long get_file_size(const std::string& filename) {
    std::ifstream f(filename, std::ios::binary);  // открываем бинарный файл для чтения
    if (!f) return -1;
    f.seekg(0, std::ios::end);                     // перемещаем указатель в конец
    std::streampos size = f.tellg();                // получаем размер в байтах
    f.close();                             
    return size / sizeof(int);                      // делим на размер int => количество чисел
}

void N_3() {
    std::setlocale(LC_ALL, "ru");  

    // Ввод имени архивного файла
    std::string S0;
    std::cout << "Введите имя выходного файла-архива: ";
    std::getline(std::cin, S0);

    int N;
    std::cout << "Введите количество файлов для архивации (N <= 4): ";
    std::cin >> N;
    std::cin.ignore();   // очищаем буфер после ввода числа


    if (N < 0 || N > 4) {
        std::cerr << "Ошибка: N должно быть от 0 до 4.\n";
        std::exit(1); 
    }

    //Создание выходного файла-архива
    std::ofstream fout(S0, std::ios::binary);
    if (!fout) {
        std::cerr << "Ошибка создания файла " << S0 << std::endl;
        std::exit(1);
    }

    int total_files_processed = 0;   // счётчик успешно обработанных файлов
    long long total_elements = 0;     // общее количество чисел в архиве

    // Цикл по всем исходным файлам
    for (int i = 1; i <= N; ++i) {
        std::string Si;
        std::cout << "Введите имя файла N " << i << ": ";
        std::getline(std::cin, Si);

        // Получаем количество чисел в текущем файле
        long long count = get_file_size(Si);
        if (count == -1) {
            std::cerr << "Ошибка открытия файла " << Si << ". Пропускаем.\n";
            continue;   // пропускаем этот файл, переходим к следующему
        }

        // Открываем исходный файл для чтения
        std::ifstream fin(Si, std::ios::binary);
        if (!fin) {
            std::cerr << "Не удалось открыть файл " << Si << " для чтения. Пропускаем.\n";
            continue;
        }

        // Записываем в архив размер этого файла (количество чисел)
        int size_to_write = static_cast<int>(count);  
        fout.write((char*)&size_to_write, sizeof(int));

        // Копируем все числа из исходного файла в архив
        int num;
        for (long long j = 0; j < count; ++j) {
            fin.read((char*)&num, sizeof(num));        // читаем одно число из исходного файла
            if (!fin) {                                 // если чтение не удалось
                std::cerr << "Ошибка чтения из файла " << Si << " на элементе " << j + 1 << std::endl;
                break;                                 
            }
            fout.write((char*)&num, sizeof(num));       // записываем число в архив
        }

        fin.close();                // закрываем исходный файл
        total_files_processed++;     // увеличиваем счётчик обработанных файлов
        total_elements += count;     // добавляем числа к общему количеству
        std::cout << "Файл " << Si << " добавлен (" << count << " чисел).\n";
    }

    fout.close();

    // Вывод итоговой информации
    std::cout << "\nАрхив " << S0 << " успешно создан.\n";
    std::cout << "Обработано файлов: " << total_files_processed << "\n";
    std::cout << "Всего чисел в архиве: " << total_elements << "\n";

    std::cout << "\nХотите просмотреть содержимое архива? (y/n): ";
    char ans;
    std::cin >> ans;
    if (ans == 'y' || ans == 'Y') {
        print_bin_file(S0);
    }
}

// Задача 4

int parseExpression(const std::string& s, std::size_t& pos);

// Возвращает обновлённый результат после обработки всех оставшихся аргументов.
int parseParams(const std::string& s, std::size_t& pos, int current, char func) {
    // Если достигнут конец строки или текущий символ не запятая, значит аргументов больше нет,
    // возвращаем текущий результат.
    if (pos >= s.size() || s[pos] != ',') {
        return current;
    }
    ++pos; // пропускаем запятую

    // Рекурсивно вызываем parseExpression для получения значения следующего аргумента.
    int next = parseExpression(s, pos);

    // Обновляем current в зависимости от типа функции.
    if (func == 'M') {
        if (next > current) current = next; // для максимума оставляем большее
    }
    else { // 'm'
        if (next < current) current = next; // для минимума оставляем меньшее
    }

    // Рекурсивно вызываем parseParams для обработки оставшихся после текущего аргументов.
    return parseParams(s, pos, current, func);
}

// Разбирает выражение, начиная с текущей позиции pos, и возвращает его значение.
int parseExpression(const std::string& s, std::size_t& pos) {
   
    if (pos >= s.size()) {  //Проверка не вышли ли за пределы
        std::cerr << "Ошибка: неожиданный конец строки" << std::endl;
        return 0;
    }

    char ch = s[pos]; // текущий символ

    //Случай 1 цифра 
    if (std::isdigit(ch)) {
        ++pos;
        return ch - '0';
    }

    //Случай 2 функция M или m
    if (ch == 'M' || ch == 'm') {
        char func = ch;       // запоминаем тип функции
        ++pos;                // переходим к следующему символу после M/m

        // Ожидаем открывающую скобку (
        if (pos >= s.size() || s[pos] != '(') {
            std::cerr << "Ошибка: ожидается '('" << std::endl;
            return 0;
        }
        ++pos; // пропускаем (

        // Рекурсивно вычисляем первый аргумент
        int result = parseExpression(s, pos);  

        // Обрабатываем остальные аргументы которые через запятые
        // parseParams будет вызывать parseExpression для каждого следующего аргумента
        // и обновлять result согласно func.
        result = parseParams(s, pos, result, func);

        // После обработки всех аргументов должна быть закрывающая скобка )
        if (pos >= s.size() || s[pos] != ')') {
            std::cerr << "Ошибка: ожидается ')'" << std::endl;
            return 0;
        }
        ++pos; // пропускаем ')'
        return result; // возвращаем итоговое значение
    }

    //Случай 3 недопустимый символ ошибка
    std::cerr << "Ошибка недопустимый символ '" << ch << "'" << std::endl;
    return 0;
}

void N_4() {
    std::setlocale(LC_ALL, "ru");

    std::string s;
    std::cout << "Введите выражение: ";
    std::getline(std::cin, s);

    std::size_t pos = 0;
    int result = parseExpression(s, pos);

    if (pos < s.size()) {
        std::cout << "Предупреждение после выражения остались символы." << std::endl;
    }

    std::cout << "Результат: " << result << std::endl;
}

// Дополнительные функции 

// Универсальная функция для просмотра бинарного файла.
void printBinaryFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Ошибка не удалось открыть файл " << filename << std::endl;
        return;
    }

    int num;
    std::cout << "Содержимое файла " << filename << ":\n";
    bool empty = true;
    while (file.read((char*)&num, sizeof(num))) {
        std::cout << num << " ";
        empty = false;
    }
    if (empty) std::cout << "(файл пуст)";
    std::cout << std::endl;
    file.close();
}

void N_5() {
    setlocale(LC_ALL, "ru");

    std::string filename;
    std::cout << "Введите имя бинарного файла для просмотра: ";
    std::getline(std::cin, filename);
    if (filename.empty()) {
        std::cout << "Имя не введено, используется значение по умолчанию 'data.bin'.\n";
        filename = "data.bin";
    }
    printBinaryFile(filename);
}

// Универсальная функция для создания бинарного файла.
void N_6() {
    std::setlocale(LC_ALL, "ru");

    char choice;
    do {
        std::string filename;
        std::cout << "Введите имя бинарного файла для создания: ";
        std::getline(std::cin, filename);
        if (filename.empty()) {
            std::cerr << "Имя файла не может быть пустым. Пропуск.\n";
            continue;
        }

        int count;
        std::cout << "Введите количество целых чисел для записи: ";
        std::cin >> count;
        if (count <= 0) {
            std::cerr << "Количество должно быть положительным. Пропуск.\n";
            std::cin.ignore(10000, '\n');
            continue;
        }

        std::ofstream fout(filename, std::ios::binary);
        if (!fout) {
            std::cerr << "Ошибка создания файла " << filename << std::endl;
            std::cin.ignore(10000, '\n');
            continue;
        }

        std::cout << "Введите " << count << " целых чисел:\n";
        int num;
        for (int i = 0; i < count; ++i) {
            std::cout << "Число " << i + 1 << ": ";
            std::cin >> num;
            fout.write((char*)&num, sizeof(num));
        }

        fout.close();
        std::cout << "Файл " << filename << " успешно создан с " << count << " числами.\n";

        std::cout << "Хотите создать ещё один файл? (y/n): ";
        std::cin >> choice;
        std::cin.ignore(10000, '\n');
    } while (choice == 'y' || choice == 'Y');

    std::cout << "Программа завершена.\n";
}