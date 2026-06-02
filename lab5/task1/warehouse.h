#ifndef WAREHOUSE_H
#define WAREHOUSE_H

#include <string>
#include <map>
#include <vector>

// Структура для хранения данных о товаре в ячейке
struct Cell {
    std::string itemName = "";
    int count = 0;
};

class WarehouseManager {
private:
    // Контейнер склада: Адрес ячейки (string) -> Данные ячейки (Cell)
    std::map<std::string, Cell> warehouse;

    // Константы конфигурации для Варианта 2
    const int ZONES_COUNT = 2;       // А и Б
    const int STILLAGE = 15;   // 15 стеллажей (1..15)
    const int SECTIONS_VERTIC = 3; // 3 секции (1..3)
    const int POLKY = 5;// 5 полок (1..5)
    const int MAX_CELL_CAPACITY = 10;// Максимум 10 единиц в ячейке
    
    const int TOTAL_CELLS = 2 * 15 * 3 * 5; // 450 ячеек
    const int TOTAL_CAPACITY = TOTAL_CELLS * 10; // 4500 единиц товара

    // Вспомогательные методы
    bool isValidAddress(const std::string& address) const;
    std::vector<std::string> getAllPossibleAddresses() const;

public:
    WarehouseManager();

    void executeCommand(const std::string& fullCommand);
    void addGoods(const std::string& itemName, int count, const std::string& address);
    void removeGoods(const std::string& itemName, int count, const std::string& address);
    void printInfo() const;
};

#endif // WAREHOUSE_H