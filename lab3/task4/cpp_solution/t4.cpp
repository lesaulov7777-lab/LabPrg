#include <iostream>
#include <vector>
#include <map>

using namespace std;

int main() {
    setlocale(LC_ALL, "Russian");
    
    cout << "--- Задание 4: Таблица простых чисел ---" << endl;
    int n, m;
    cout << "Введите количество чисел (N) и ограничение на ход (M) через пробел: ";
    if (!(cin >> n >> m)) {
        cout << "Ошибка ввода параметров!" << endl;
        return 1;
    }

    vector<int> numbers(n);
    cout << "Введите " << n << " чисел(ла) через пробел или Enter: ";
    for (int i = 0; i < n; i++) {
        cin >> numbers[i];
    }

    map<pair<int, int>, bool> memo;

    for (int rem = 0; rem <= n; ++rem) {
        for (int last = 0; last <= m; ++last) {
            if (rem == 0) {
                memo[{rem, last}] = false;
                continue;
            }

            bool current_state_win = false;

            for (int take = 1; take <= m; ++take) {
                if (take <= rem && take != last) {
                    pair<int, int> next_state = {rem - take, take};
                    if (memo.count(next_state) && !memo[next_state]) {
                        current_state_win = true;
                        break;
                    }
                }
            }
            memo[{rem, last}] = current_state_win;
        }
    }

    cout << "---------------------------------------" << endl;
    cout << "Результат игры (1 - Павел, 0 - Вика): ";

    if (memo[{n, 0}]) {
        cout << 1 << " (Выиграл Павел)" << endl;
    } else {
        cout << 0 << " (Выиграла Вика)" << endl;
    }
    
    return 0;
}