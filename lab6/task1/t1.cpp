#include <iostream>
#include <vector>
#include <clocale>

int main() {
    // Настройка локализации для корректного отображения кириллицы
    std::setlocale(LC_ALL, "Russian");

    int M, N;
    std::cout << "Введите количество строк (M): ";
    if (!(std::cin >> M) || M <= 0) {
        std::cerr << "Ошибка: некорректное количество строк!" << std::endl;
        return 1;
    }

    std::cout << "Введите количество столбцов (N): ";
    if (!(std::cin >> N) || N <= 0) {
        std::cerr << "Ошибка: некорректное количество столбцов!" << std::endl;
        return 1;
    }

    // Создание двумерного вектора (матрицы) размера M x N
    std::vector<std::vector<int>> matrix(M, std::vector<int>(N));

    // Ввод элементов матрицы
    std::cout << "Введите элементы матрицы построчно:" << std::endl;
    for (int i = 0; i < M; ++i) {
        std::cout << "Строка " << i + 1 << ": ";
        for (int j = 0; j < N; ++j) {
            std::cin >> matrix[i][j];
        }
    }

    int sortedRowsCount = 0;

    // Логика проверки упорядоченности строк
    for (int i = 0; i < M; ++i) {
        bool isSorted = true;
        
        // Одиночный элемент или пустая строка формально считаются упорядоченными
        for (int j = 0; j < N - 1; ++j) {
            // Если текущий элемент больше следующего — порядок нарушен.
            // Примечание: если преподаватель требует СТРОГОЕ возрастание 
            // (без повторяющихся элементов), замените '>' на '>='
            if (matrix[i][j] > matrix[i][j + 1]) {
                isSorted = false;
                break; // Прерываем проверку текущей строки
            }
        }
        
        if (isSorted) {
            sortedRowsCount++;
        }
    }

    // Вывод матрицы на экран для проверки
    std::cout << "\nВведенная матрица:" << std::endl;
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < N; ++j) {
            std::cout << matrix[i][j] << "\t";
        }
        std::cout << std::endl;
    }

    // Вывод итогового результата
    std::cout << "\nКоличество строк, упорядоченных по возрастанию: " << sortedRowsCount << std::endl;

    return 0;
}