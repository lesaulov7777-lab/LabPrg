#ifndef TRAM_SYSTEM_H
#define TRAM_SYSTEM_H

#pragma once

#include <string>
#include <vector>
#include <map>
#include <set>
#include <iostream>

// Перечисление для хранения типов команд
enum class CommandType {
    CREATE_TRAM,
    TRAMS_IN_STOP,
    STOPS_IN_TRAM,
    TRAMS,
    UNKNOWN
};

// Функция для сопоставления строки с типом перечисления
CommandType ParseCommand(const std::string& commandStr);

class TramSystem {
private:
    // Хранилище: имя трамвая -> список его остановок
    // Использование std::map автоматически сортирует трамваи по имени (лексикографически)
    std::map<std::string, std::vector<std::string>> trams;

    // Вспомогательный метод для проверки существования остановки
    bool StopExists(const std::string& stop) const;

public:
    // Основной метод диспетчеризации команд
    void ExecuteCommand(CommandType type, std::istream& is);

    // Функции обработки отдельных запросов
    void CreateTram(std::istream& is);
    void PrintTramsInStop(std::istream& is);
    void PrintStopsInTram(std::istream& is);
    void PrintAllTrams();
};

#endif // TRAM_SYSTEM_H