#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>

using namespace std;

string findLongestChainEnd(const vector<pair<string, string>>& routes) {
    if (routes.empty()) return "Ошибка";

    unordered_map<string, string> nextPoint;   // from -> to
    unordered_map<string, int> indegree;       // сколько раз в этот пункт приходят
    unordered_set<string> allNodes;

    // 1) Валидация и построение графа
    for (const auto& [from, to] : routes) {
        // Пример 3: Искитим -> Искитим => ошибка
        if (from == to) return "Ошибка";

        // Из одного пункта не должно быть двух разных переходов
        if (nextPoint.count(from) && nextPoint[from] != to) return "Ошибка";
        nextPoint[from] = to;

        indegree[to]++;
        allNodes.insert(from);
        allNodes.insert(to);
    }

    // 2) Ищем все начала цепочек (вершины без входящих ребер)
    vector<string> starts;
    for (const auto& node : allNodes) {
        if (!indegree.count(node)) {
            // node с indegree == 0
            if (nextPoint.count(node)) starts.push_back(node); // начало реальной цепи
        }
    }

    // Если стартов нет, а ребра есть => цикл
    if (starts.empty()) return "Ошибка";

    // 3) Проходим каждую цепочку, ищем самую длинную
    int bestLen = -1;
    string bestEnd = "Ошибка";

    unordered_set<string> usedEdgesStarts; // чтобы потом проверить, что все ребра покрыты

    for (const string& s : starts) {
        unordered_set<string> localVisited;
        string cur = s;
        int len = 0; // число переходов в цепочке

        while (nextPoint.count(cur)) {
            // цикл в рамках текущего прохода
            if (localVisited.count(cur)) return "Ошибка";
            localVisited.insert(cur);

            usedEdgesStarts.insert(cur);
            cur = nextPoint[cur];
            len++;
        }

        // cur — конечный пункт этой цепочки
        if (len > bestLen) {
            bestLen = len;
            bestEnd = cur;
        }
    }

    // 4) Если есть ребра, не достижимые из стартов => где-то цикл/некорректный компонент
    if ((int)usedEdgesStarts.size() != (int)nextPoint.size()) return "Ошибка";

    return bestEnd;
}

int main() {
    vector<pair<string, string>> ex1 = {
        {"Лондон", "Стамбул"},
        {"Новосибирск", "Лондон"},
        {"Стамбул", "Пекин"}
    };
    vector<pair<string, string>> ex2 = {
        {"Барнаул", "Новосибирск"}
    };
    vector<pair<string, string>> ex3 = {
        {"Искитим", "Искитим"}
    };
    vector<pair<string, string>> ex4 = {
        {"Москва", "Норильск"},
        {"Норильск", "Саратов"},
        {"Саратов", "Москва"}
    };

    cout << findLongestChainEnd(ex1) << "\n"; // Пекин
    cout << findLongestChainEnd(ex2) << "\n"; // Новосибирск
    cout << findLongestChainEnd(ex3) << "\n"; // Ошибка
    cout << findLongestChainEnd(ex4) << "\n"; // Ошибка

    return 0;
}
