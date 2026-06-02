#include <iostream>
#include <string>
#include <vector>
#include "region_manager.h"

int main() {
    RegionDatabase db;
    int n = 0;

    std::cout << "Введите количество запросов (N):\n>>> ";
    if (!(std::cin >> n) || n <= 0) {
        return 0;
    }

    // Создаем фиктивный вектор размера N, чтобы использовать Range-based for loop
    std::vector<int> iterations(n, 0);

    for (int dummy : iterations) {
        std::cout << ">>> ";
        std::string command;
        std::cin >> command;

        if (command == "CHANGE") {
            std::string region, center;
            std::cin >> region >> center;
            handleChange(db, region, center);
        } else if (command == "RENAME") {
            std::string old_center, new_center;
            std::cin >> old_center >> new_center;
            handleRename(db, old_center, new_center);
        } else if (command == "ABOUT") {
            std::string region;
            std::cin >> region;
            handleAbout(db, region);
        } else if (command == "ALL") {
            handleAll(db);
        } else {
            std::cout << "Ошибка: Неизвестная команда\n";
        }
    }

    return 0;
}