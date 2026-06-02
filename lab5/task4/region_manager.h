#pragma once

#include <string>
#include <map>
#include <vector>

// Структура, объединяющая комбинацию контейнеров для учета регионов
struct RegionDatabase {
    std::map<std::string, std::string> region_to_center;
    std::map<std::string, std::string> center_to_region;
    std::vector<std::string> insertion_order;
};

// Объявления функций для обработки запросов
void handleChange(RegionDatabase& db, const std::string& region, const std::string& center);
void handleRename(RegionDatabase& db, const std::string& old_center, const std::string& new_center);
void handleAbout(const RegionDatabase& db, const std::string& region);
void handleAll(const RegionDatabase& db);