#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>

using namespace std;

int N, M;

// Функция для красивого вывода маски в виде доски в терминал
void print_matrix(int mask) {
    for (int i = 0; i < N; ++i) {
        cout << "  ";
        for (int j = 0; j < M; ++j) {
            int bit = (mask >> (i * M + j)) & 1;
            cout << bit << " ";
        }
        cout << "\n";
    }
}

// Генерация целевых состояний
pair<int, int> get_target_masks() {
    int maskA = 0; // По краю 1, внутри 0
    int maskB = 0; // По краю 0, внутри 1
    
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            int bit_pos = i * M + j;
            bool is_border = (i == 0 || i == N - 1 || j == 0 || j == M - 1);
            
            if (is_border) {
                maskA |= (1 << bit_pos);
            } else {
                maskB |= (1 << bit_pos);
            }
        }
    }
    return {maskA, maskB};
}

int solve(int start_mask) {
    auto [targetA, targetB] = get_target_masks();
    
    if (start_mask == targetA || start_mask == targetB) {
        return 0;
    }
    
    queue<int> q;
    unordered_map<int, int> dist;
    
    q.push(start_mask);
    dist[start_mask] = 0;
    
    while (!q.empty()) {
        int curr = q.front();
        q.pop();
        int curr_dist = dist[curr];
        
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < M; ++j) {
                int p1 = i * M + j;
                
                // Сосед справа
                if (j + 1 < M) {
                    int p2 = i * M + (j + 1);
                    int next_mask = curr ^ (1 << p1) ^ (1 << p2);
                    if (next_mask == targetA || next_mask == targetB) return curr_dist + 1;
                    if (dist.find(next_mask) == dist.end()) {
                        dist[next_mask] = curr_dist + 1;
                        q.push(next_mask);
                    }
                }
                // Сосед снизу
                if (i + 1 < N) {
                    int p2 = (i + 1) * M + j;
                    int next_mask = curr ^ (1 << p1) ^ (1 << p2);
                    if (next_mask == targetA || next_mask == targetB) return curr_dist + 1;
                    if (dist.find(next_mask) == dist.end()) {
                        dist[next_mask] = curr_dist + 1;
                        q.push(next_mask);
                    }
                }
            }
        }
    }
    return -1;
}

int main() {
    // Настройка локализации для корректного вывода кириллицы
    setlocale(LC_ALL, "Russian");

    cout << "=========================================================\n";
    cout << "   ПРОГРАММА ПОИСКА МИНИМАЛЬНОГО ПУТИ К РАМКЕ МОНЕТ\n";
    cout << "=========================================================\n\n";

    cout << "[Шаг 1] Введите размеры щита (N - строки, M - столбцы):\n";
    cout << "Формат ввода: N M (например, 3 3): ";
    if (!(cin >> N >> M) || N <= 0 || M <= 0) {
        cout << "\n[Ошибка] Некорректный размер доски!\n";
        return 1;
    }

    // Проверка ограничений для битовой маски (максимум 30-32 бита для стандартного int)
    if (N * M > 30) {
        cout << "\n[Предупреждение] Размер доски слишком велик для BFS перебора масок (> 30 клеток).\n";
        return 1;
    }

    // Показываем пользователю, какие рамки будут считаться целью
    auto [tA, tB] = get_target_masks();
    cout << "\n[Инфо] Целевые состояния («Рамки») для размера " << N << "x" << M << ":\n";
    cout << "Вариант А (1 по краям):\n";
    print_matrix(tA);
    cout << "Вариант Б (0 по краям):\n";
    print_matrix(tB);
    cout << "---------------------------------------------------------\n\n";

    cout << "[Шаг 2] Введите матрицу монет (строка за строкой).\n";
    cout << "Используйте '0' для орла и '1' для решки:\n";
    
    int start_mask = 0;
    for (int i = 0; i < N; ++i) {
        cout << "Строка " << i + 1 << " (введите " << M << " чисел через пробел): ";
        for (int j = 0; j < M; ++j) {
            int val;
            cin >> val;
            if (val != 0 && val != 1) {
                cout << "\n[Ошибка] Разрешены только значения 0 или 1!\n";
                return 1;
            }
            if (val == 1) {
                start_mask |= (1 << (i * M + j));
            }
        }
    }

    cout << "\n---------------------------------------------------------\n";
    cout << "[Статус] Исходная доска принята. Запускаю расчет алгоритма BFS...\n";
    cout << "---------------------------------------------------------\n\n";

    int result = solve(start_mask);

    cout << "==================== РЕЗУЛЬТАТ ==========================\n";
    if (result == -1) {
        cout << " Исходное состояние: \n";
        print_matrix(start_mask);
        cout << "\n ПРИГОВОР: Из данного расположения получить рамку НЕВОЗМОЖНО.\n";
    } else {
        cout << " УСПЕХ! Целевой рисунок достижим.\n";
        cout << " Минимальное количество ходов (переворотов пар): " << result << "\n";
    }
    cout << "=========================================================\n";

    return 0;
}