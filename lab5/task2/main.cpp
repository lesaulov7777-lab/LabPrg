#include "port_management.h"
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

int main() {
    // Настройка локализации для корректного вывода на русском языке
    std::setlocale(LC_ALL, "Russian");

    int max_stack_weight = 0;
    int section_count = 0;

    std::cout << "Введите максимальный размер стека:\n>>> ";
    if (!(std::cin >> max_stack_weight)) return 0;

    std::cout << "Введите количество секций судна:\n>>> ";
    if (!(std::cin >> section_count)) return 0;

    std::vector<ContainerStack> port_stacks;
    std::string line;
    
    // Очищаем буфер после ввода чисел
    std::getline(std::cin, line); 

    // Цикл обработки пользовательского ввода
    while (true) {
        std::cout << ">>> ";
        if (!std::getline(std::cin, line) || line.empty()) {
            break;
        }

        std::stringstream ss(line);
        std::string command;
        ss >> command;

        if (command == "ARRIVE") {
            std::string id;
            int weight;
            if (ss >> id >> weight) {
                process_arrive(port_stacks, id, weight, max_stack_weight);
            } else {
                std::cout << "Ошибка: Неверный формат команды ARRIVE\n";
            }
        } 
        else if (command == "LOAD") {
            process_load(port_stacks, section_count);
            break; // После команды LOAD выполнение симуляции завершается по логике примера
        } 
        else {
            std::cout << "Ошибка: Неизвестная команда\n";
        }
    }

    return 0;
}