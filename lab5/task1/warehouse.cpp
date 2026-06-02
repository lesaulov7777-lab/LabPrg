#include "warehouse.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cctype>

WarehouseManager::WarehouseManager() {
    // Инициализируем карту всеми возможными адресами согласно Варианту 2
    for (std::string address : getAllPossibleAddresses()) {
        warehouse[address] = Cell();
    }
}

// Генерация всех валидных адресов для Варианта 2
std::vector<std::string> WarehouseManager::getAllPossibleAddresses() const {
    std::vector<std::string> addresses;
    char zones[] = {'A', 'B'}; // 2 зоны: А=1-я, Б=2-я (используем латиницу для стабильности ввода)
    
    for (char zone : zones) {
        for (int rack = 1; rack <= STILLAGE; ++rack) {
            for (int section = 1; section <= SECTIONS_VERTIC; ++section) {
                for (int shelf = 1; shelf <= POLKY; ++shelf) {
                    std::string addr = zone + std::to_string(rack) + std::to_string(section) + std::to_string(shelf);
                    addresses.push_back(addr);
                }
            }
        }
    }
    return addresses;
}

// Валидация адреса под формат Варианта 2
bool WarehouseManager::isValidAddress(const std::string& address) const {
    return warehouse.find(address) != warehouse.end();
}

void WarehouseManager::executeCommand(const std::string& fullCommand) {
    std::stringstream ss(fullCommand);
    std::string action;
    ss >> action;

    if (action == "ADD") {
        std::string itemName, address;
        int count;
        if (ss >> itemName >> count >> address) {
            addGoods(itemName, count, address);
        } else {
            std::cout << "Ошибка: Неверный формат команды ADD\n";
        }
    } else if (action == "REMOVE") {
        std::string itemName, address;
        int count;
        if (ss >> itemName >> count >> address) {
            removeGoods(itemName, count, address);
        } else {
            std::cout << "Ошибка: Неверный формат команды REMOVE\n";
        }
    } else if (action == "INFO") {
        printInfo();
    } else {
        std::cout << "Ошибка: Неизвестная команда\n";
    }
}

void WarehouseManager::addGoods(const std::string& itemName, int count, const std::string& address) {
    if (!isValidAddress(address)) {
        std::cout << "Ошибка: Несуществующий адрес ячейки для данной конфигурации\n";
        return;
    }

    Cell& cell = warehouse[address];

    if (cell.count > 0 && cell.itemName != itemName) {
        std::cout << "Ошибка: Ячейка " << address << " занята товаром " << cell.itemName << "\n";
        return;
    }

    if (cell.count + count > MAX_CELL_CAPACITY) {
        std::cout << "Ошибка: Превышена вместимость ячейки (максимум " << MAX_CELL_CAPACITY << ")\n";
        return;
    }

    cell.itemName = itemName;
    cell.count += count;
    std::cout << "Добавлено " << count << " " << itemName << " в " << address << "\n";
}

void WarehouseManager::removeGoods(const std::string& itemName, int count, const std::string& address) {
    if (!isValidAddress(address)) {
        std::cout << "Ошибка: Несуществующий адрес ячейки для данной конфигурации\n";
        return;
    }

    Cell& cell = warehouse[address];

    if (cell.count == 0 || cell.itemName != itemName) {
        std::cout << "Ошибка: Товар " << itemName << " не найден в ячейке " << address << "\n";
        return;
    }

    if (cell.count < count) {
        std::cout << "Ошибка: Недостаточно товаров для удаления\n";
        return;
    }

    cell.count -= count;
    std::cout << "Удалено " << count << " " << itemName << " (остаток: " << cell.count << ")\n";

    if (cell.count == 0) {
        cell.itemName = "";
    }
}

void WarehouseManager::printInfo() const {
    int totalItems = 0;
    int itemsZoneA = 0;
    int itemsZoneB = 0;

    std::vector<std::string> filledCells;
    std::vector<std::string> emptyCells;

    // Использование строго Range-based for loop для обхода std::map
    for (const auto& pair : warehouse) {
        std::string addr = pair.first;
        Cell cell = pair.second;

        if (cell.count > 0) {
            totalItems += cell.count;
            if (addr[0] == 'A') itemsZoneA += cell.count;
            if (addr[0] == 'B') itemsZoneB += cell.count;

            filledCells.push_back(addr + ": " + cell.itemName + " (" + std::to_string(cell.count) + ")");
        } else {
            emptyCells.push_back(addr);
        }
    }

    // Расчет вместимости для одной зоны (половина общего объема)
    double capacityPerZone = TOTAL_CAPACITY / 2.0;

    double totalLoadPercent = (static_cast<double>(totalItems) / TOTAL_CAPACITY) * 100.0;
    double zoneALoadPercent = (static_cast<double>(itemsZoneA) / capacityPerZone) * 100.0;
    double zoneBLoadPercent = (static_cast<double>(itemsZoneB) / capacityPerZone) * 100.0;

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Загруженность склада: " << totalLoadPercent << "%\n";
    std::cout << "Загруженность зоны A: " << zoneALoadPercent << "%\n";
    std::cout << "Загруженность зоны B: " << zoneBLoadPercent << "%\n";

    std::cout << "Заполненные ячейки:\n";
    if (filledCells.empty()) {
        std::cout << "  (нет заполненных ячеек)\n";
    } else {
        for (const std::string& info : filledCells) {
            std::cout << "  " << info << "\n";
        }
    }

    std::cout << "Пустые ячейки:\n  ";
    for (size_t i = 0; i < emptyCells.size(); ++i) {
        std::cout << emptyCells[i] << (i == emptyCells.size() - 1 ? "" : ", ");
        if ((i + 1) % 10 == 0 && i != emptyCells.size() - 1) {
            std::cout << "\n  "; // Форматирование вывода адресов списком по 10 штук
        }
    }
    std::cout << "\n";
}