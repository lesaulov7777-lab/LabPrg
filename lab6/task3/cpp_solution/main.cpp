#include <iostream>
#include "gauss.hpp"
#include "seidel.hpp"

int main() {
    // Настройка локализации для корректного вывода на русском языке
    std::setlocale(LC_ALL, "Russian");
    
    int choice;
    do {
        std::cout << "=================================================" << std::endl;
        std::cout << "Лабораторная работа. Задание 3 (Вариант 16)" << std::endl;
        std::cout << "Выберите метод решения СЛАУ:" << std::endl;
        std::cout << "1. Прямой метод (Метод Гаусса с выбором главного элемента)" << std::endl;
        std::cout << "2. Приближенный метод (Метод Зейделя, e = 10^-3)" << std::endl;
        std::cout << "3. Запустить оба метода последовательно" << std::endl;
        std::cout << "0. Выход" << std::endl;
        std::cout << "Ваш выбор: ";
        std::cin >> choice;
        std::cout << "=================================================" << std::endl << std::endl;

        switch (choice) {
            case 1:
                solveGauss();
                break;
            case 2:
                solveSeidel();
                break;
            case 3:
                solveGauss();
                std::cout << "\n\n";
                solveSeidel();
                break;
            case 0:
                std::cout << "Завершение работы программы." << std::endl;
                break;
            default:
                std::cout << "Неверный ввод. Попробуйте еще раз." << std::endl;
        }
    } while (choice != 0);

    return 0;
}