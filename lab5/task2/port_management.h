#pragma once

#include <string>
#include <vector>

// Структура для хранения данных о контейнере
struct Container {
    std::string id;
    int weight;
};

// Структура для стека в порту (LIFO)
struct ContainerStack {
    std::vector<Container> containers;
    int current_weight = 0;
};

// Структура для секции судна
struct ShipSection {
    int id;
    int total_weight = 0;
    std::vector<std::string> container_ids;
};

// Функция для обработки прибытия контейнера
void process_arrive(std::vector<ContainerStack>& stacks, const std::string& id, int weight, int max_stack_weight);

// Функция для распределения контейнеров по секциям судна
void process_load(std::vector<ContainerStack>& stacks, int section_count);