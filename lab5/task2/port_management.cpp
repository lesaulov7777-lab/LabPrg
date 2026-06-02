#include "port_management.h"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

void process_arrive(std::vector<ContainerStack>& stacks, const std::string& id, int weight, int max_stack_weight) {
    bool placed = false;
    int stack_index = 0;

    // Ищем первый по порядку стек, куда поместится контейнер
    for (size_t i = 0; i < stacks.size(); ++i) {
        if (stacks[i].current_weight + weight <= max_stack_weight) {
            stacks[i].containers.push_back({id, weight});
            stacks[i].current_weight += weight;
            stack_index = static_cast<int>(i) + 1;
            placed = true;
            break;
        }
    }

    // Если ни один стек не подошел, создаем новый
    if (!placed) {
        ContainerStack new_stack;
        new_stack.containers.push_back({id, weight});
        new_stack.current_weight = weight;
        stacks.push_back(new_stack);
        stack_index = static_cast<int>(stacks.size());
    }

    std::cout << "Контейнер " << id << " размещен в стек " << stack_index << "\n";
}

void process_load(std::vector<ContainerStack>& stacks, int section_count) {
    // Инициализируем секции судна
    std::vector<ShipSection> sections;
    for (int i = 1; i <= section_count; ++i) {
        sections.push_back({i, 0, {}});
    }

    // Извлекаем контейнеры в обратном порядке их прибытия (LIFO)
    // Для сохранения сквозного LIFO-порядка по всему порту, 
    // мы собираем верхние элементы из стеков в порядке, обратном заполнению.
    // Но так как в условии сказано "извлекаются из стеков в обратном порядке прибытия",
    // мы аккуратно вытаскиваем их из активных стеков.
    while (true) {
        bool any_left = false;
        // Ищем последний созданный стек, в котором еще есть контейнеры
        for (auto it = stacks.rbegin(); it != stacks.rend(); ++it) {
            if (!it->containers.empty()) {
                any_left = true;
                
                // Берем верхний контейнер (LIFO)
                Container current_container = it->containers.back();
                it->containers.pop_back();
                it->current_weight -= current_container.weight;

                // Ищем секцию с наименьшей текущей загрузкой
                // При равенстве выбирается секция с меньшим порядковым номером
                size_t best_section_idx = 0;
                for (size_t i = 1; i < sections.size(); ++i) {
                    if (sections[i].total_weight < sections[best_section_idx].total_weight) {
                        best_section_idx = i;
                    }
                }

                // Перемещаем контейнер в выбранную секцию
                sections[best_section_idx].total_weight += current_container.weight;
                sections[best_section_idx].container_ids.push_back(current_container.id);
                
                break; // Возвращаемся к началу цикла, чтобы снова оценить баланс секций
            }
        }
        if (!any_left) break;
    }

    // Выводим результаты работы с использованием Range-based for loop
    for (const auto& section : sections) {
        std::cout << "Секция " << section.id << " (" << section.total_weight << " тонн): ";
        for (size_t i = 0; i < section.container_ids.size(); ++i) {
            std::cout << section.container_ids[i];
            if (i + 1 < section.container_ids.size()) {
                std::cout << ", ";
            }
        }
        std::cout << "\n";
    }
}