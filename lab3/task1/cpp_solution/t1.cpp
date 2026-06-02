#include <iostream>
#include <cmath>
#include <iomanip>

using namespace std;

void task1() {
    cout << "--- Задание 1: Таблица значений функции ---" << endl;
    double dx = 0.5;
    cout << setw(10) << "x" << setw(15) << "y" << endl;
    cout << "--------------------------" << endl;

    for (double x = -6.0; x <= 8.0 + dx / 2; x += dx) {
        double y;
        if (x >= -6 && x <= -2) {
            y = -sqrt(-(x + 2));
        } else if (x > -2 && x <= -1) {
            y = sqrt(1 - pow(x + 1, 2));
        } else if (x > -1 && x <= 0) {
            y = x * x;
        } else if (x > 0 && x <= 2) {
            y = 0.5 * x * x;
        } else if (x > 2 && x <= 4) {
            y = sqrt(4 - pow(x - 2, 2));
        } else if (x > 4 && x <= 8) {
            y = -sqrt(x - 4);
        } else {
            continue;
        }
        cout << setw(10) << fixed << setprecision(2) << x << setw(15) << y << endl;
    }
}

int main() {
    setlocale(LC_ALL, "Russian");
    task1();
    return 0;
}