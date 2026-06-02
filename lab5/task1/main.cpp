#include "warehouse.h"
#include <iostream>
#include <string>

int main() {
    
    WarehouseManager manager;
    std::string commandLine;

    std::cout << "Система учета товаров на складе (Вариант 2) запущена.\n";
    std::cout << "Доступные команды: ADD, REMOVE, INFO. Для выхода введите EXIT.\n\n";

    while (true) {
        std::cout << ">>> ";
        std::getline(std::cin, commandLine);

        if (commandLine == "EXIT" || commandLine == "exit") {
            break;
        }

        if (commandLine.empty()) {
            continue;
        }

        manager.executeCommand(commandLine);
    }

    return 0;
}