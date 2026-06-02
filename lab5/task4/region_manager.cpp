#include "region_manager.h"
#include <iostream>

void handleChange(RegionDatabase& db, const std::string& region, const std::string& center) {
    if (db.region_to_center.find(region) != db.region_to_center.end()) {
        std::cout << "Ошибка: Регион " << region << " уже создан\n";
        return;
    }
    if (db.center_to_region.find(center) != db.center_to_region.end()) {
        std::cout << "Ошибка: " << center << " уже является административным центром другого региона\n";
        return;
    }

    db.region_to_center[region] = center;
    db.center_to_region[center] = region;
    db.insertion_order.push_back(region);
    
    std::cout << "Новый регион " << region << " с административным центром " << center << "\n";
}

void handleRename(RegionDatabase& db, const std::string& old_center, const std::string& new_center) {
    if (old_center == new_center) {
        std::cout << "Ошибка: Нельзя переименовать " << old_center << " в то же название\n";
        return;
    }
    
    auto it_old = db.center_to_region.find(old_center);
    if (it_old == db.center_to_region.end()) {
        std::cout << "Ошибка: Административный центр " << old_center << " не найден\n";
        return;
    }
    
    if (db.center_to_region.find(new_center) != db.center_to_region.end()) {
        std::cout << "Ошибка: " << new_center << " уже является административным центром другого региона\n";
        return;
    }

    std::string region = it_old->second;

    // Обновляем связи в контейнерах
    db.center_to_region.erase(it_old);
    db.center_to_region[new_center] = region;
    db.region_to_center[region] = new_center;

    std::cout << "Административный центр " << old_center 
              << " переименован в административный центр " << new_center << "\n";
}

void handleAbout(const RegionDatabase& db, const std::string& region) {
    auto it = db.region_to_center.find(region);
    if (it == db.region_to_center.end()) {
        std::cout << "Ошибка: Регион " << region << " не найден\n";
    } else {
        std::cout << "Регион " << region << " имеет административный центр " << it->second << "\n";
    }
}

void handleAll(const RegionDatabase& db) {
    if (db.insertion_order.empty()) {
        std::cout << "Справочник пуст\n";
        return;
    }

    bool first = true;
    // Обязательный Range-based for loop для обхода элементов
    for (const auto& region : db.insertion_order) {
        if (!first) {
            std::cout << ", ";
        }
        first = false;
        std::cout << region << " – " << db.region_to_center.at(region);
    }
    std::cout << "\n";
}