#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <limits>

using namespace std;

// Структура для хранения данных голосования
struct VotingData {
    vector<string> candidates;
    // pair<количество голосов, цепочка имен кандидатов>
    vector<pair<int, vector<string>>> voteGroups;
};

// Метод Борда
void solveBorda(const VotingData& data) {
    int n = data.candidates.size();
    map<string, int> scores;

    for (const auto& cand : data.candidates) scores[cand] = 0;

    for (const auto& group : data.voteGroups) {
        int count = group.first;
        const auto& ranking = group.second;
        for (int i = 0; i < n; ++i) {
            int points = (n - 1) - i;
            scores[ranking[i]] += points * count;
        }
    }

    cout << "\n=== РЕЗУЛЬТАТЫ МЕТОДА БОРДА ===" << endl;
    string winner;
    int max_score = -1;
    for (const auto& cand : data.candidates) {
        cout << cand << ": " << scores[cand] << " очков" << endl;
        if (scores[cand] > max_score) {
            max_score = scores[cand];
            winner = cand;
        }
    }
    cout << "Победитель по Борду: " << winner << endl;
}

// Метод Кондорсе
void solveCondorcet(const VotingData& data) {
    const auto& candidates = data.candidates;
    string winner = "не определен (парадокс Кондорсе)";

    cout << "\n=== РЕЗУЛЬТАТЫ МЕТОДА КОНДОРСЕ ===" << endl;

    for (const auto& candA : candidates) {
        bool is_overall_winner = true;

        for (const auto& candB : candidates) {
            if (candA == candB) continue;

            int winsA = 0;
            int winsB = 0;

            for (const auto& group : data.voteGroups) {
                int count = group.first;
                const auto& ranking = group.second;

                auto posA = find(ranking.begin(), ranking.end(), candA);
                auto posB = find(ranking.begin(), ranking.end(), candB);

                if (distance(ranking.begin(), posA) < distance(ranking.begin(), posB)) {
                    winsA += count;
                } else {
                    winsB += count;
                }
            }

            if (winsA <= winsB) {
                is_overall_winner = false;
            }
            cout << "Сравнение " << candA << " vs " << candB << ": " << winsA << " - " << winsB << endl;
        }

        if (is_overall_winner) {
            winner = candA;
            break;
        }
    }
    cout << "Победитель по Кондорсе: " << winner << endl;
}

int main() {
    setlocale(LC_ALL, "Russian");
    VotingData data;
    int n, k_groups;

    cout << "Введите количество кандидатов: ";
    cin >> n;
    data.candidates.resize(n);
    cout << "Введите имена кандидатов через пробел: ";
    for (int i = 0; i < n; ++i) {
        cin >> data.candidates[i];
    }

    cout << "Введите количество групп избирателей с одинаковыми предпочтениями: ";
    cin >> k_groups;

    for (int g = 0; g < k_groups; ++g) {
        int count;
        cout << "\nГруппа №" << g + 1 << ". Введите количество избирателей в группе: ";
        cin >> count;

        cout << "Введите их ранжирование (цепочку имен через пробел от лучшего к худшему):" << endl;
        vector<string> ranking(n);
        for (int i = 0; i < n; ++i) {
            cin >> ranking[i];
        }
        data.voteGroups.push_back({count, ranking});
    }

    solveBorda(data);
    solveCondorcet(data);

    return 0;
}