#include "TramSystem.h"

CommandType ParseCommand(const std::string& commandStr) {
    if (commandStr == "CREATE_TRAM") return CommandType::CREATE_TRAM;
    if (commandStr == "TRAMS_IN_STOP") return CommandType::TRAMS_IN_STOP;
    if (commandStr == "STOPS_IN_TRAM") return CommandType::STOPS_IN_TRAM;
    if (commandStr == "TRAMS") return CommandType::TRAMS;
    return CommandType::UNKNOWN;
}

bool TramSystem::StopExists(const std::string& stop) const {
    for (const auto& pair : trams) {
        for (const auto& s : pair.second) {
            if (s == stop) {
                return true;
            }
        }
    }
    return false;
}

void TramSystem::CreateTram(std::istream& is) {
    std::string tram;
    int n;
    if (!(is >> tram >> n)) return;

    std::vector<std::string> stops(n);
    for (auto& stop : stops) {
        is >> stop;
    }

    if (trams.count(tram)) {
        std::cout << "Ошибка: Трамвай с именем " << tram << " уже создан\n";
        return;
    }
    if (n == 1) {
        std::cout << "Ошибка: Трамвай не может быть создан с одной остановкой\n";
        return;
    }

    // Проверка на дубликаты остановок с помощью range-based цикла
    std::set<std::string> unique_stops;
    bool has_duplicates = false;
    for (const auto& stop : stops) {
        if (unique_stops.count(stop)) {
            has_duplicates = true;
            break;
        }
        unique_stops.insert(stop);
    }

    if (has_duplicates) {
        std::cout << "Ошибка: Трамвай не может быть создан с двумя одинаковыми остановками\n";
        return;
    }

    trams[tram] = stops;
    std::cout << "Трамвай " << tram << " создан\n";
}

void TramSystem::PrintTramsInStop(std::istream& is) {
    std::string stop;
    if (!(is >> stop)) return;

    if (!StopExists(stop)) {
        std::cout << "Ошибка: Остановка " << stop << " не найдена\n";
        return;
    }

    std::cout << "Трамваи на остановке " << stop << ": ";
    bool first = true;
    for (const auto& pair : trams) {
        bool passes = false;
        for (const auto& s : pair.second) {
            if (s == stop) {
                passes = true;
                break;
            }
        }
        if (passes) {
            if (!first) {
                std::cout << ", ";
            }
            std::cout << pair.first;
            first = false;
        }
    }
    std::cout << "\n";
}

void TramSystem::PrintStopsInTram(std::istream& is) {
    std::string tram;
    if (!(is >> tram)) return;

    if (!trams.count(tram)) {
        std::cout << "Ошибка: Трамвай " << tram << " не найден\n";
        return;
    }

    const auto& stops = trams.at(tram);
    std::cout << "Остановки трамвая " << tram << ":";
    for (const auto& stop : stops) {
        std::cout << " " << stop;
    }
    std::cout << "\n";

    for (const auto& stop : stops) {
        std::cout << "Остановка " << stop << ": ";
        bool first = true;
        for (const auto& pair : trams) {
            if (pair.first == tram) continue; // Исключаем сам текущий трамвай

            bool passes = false;
            for (const auto& s : pair.second) {
                if (s == stop) {
                    passes = true;
                    break;
                }
            }
            if (passes) {
                if (!first) {
                    std::cout << ", ";
                }
                std::cout << pair.first;
                first = false;
            }
        }
        if (first) {
            std::cout << "-";
        }
        std::cout << "\n";
    }
}

void TramSystem::PrintAllTrams() {
    if (trams.empty()) {
        std::cout << "Ошибка: Трамваи не найдены\n";
        return;
    }

    for (const auto& pair : trams) {
        std::cout << "Трамвай " << pair.first << ":";
        for (const auto& stop : pair.second) {
            std::cout << " " << stop;
        }
        std::cout << "\n";
    }
}

void TramSystem::ExecuteCommand(CommandType type, std::istream& is) {
    switch (type) {
        case CommandType::CREATE_TRAM:
            CreateTram(is);
            break;
        case CommandType::TRAMS_IN_STOP:
            PrintTramsInStop(is);
            break;
        case CommandType::STOPS_IN_TRAM:
            PrintStopsInTram(is);
            break;
        case CommandType::TRAMS:
            PrintAllTrams();
            break;
        case CommandType::UNKNOWN:
        default:
            std::cout << "Ошибка: Неизвестная команда\n";
            break;
    }
}