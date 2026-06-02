#include <iostream>

using namespace std;

// Собственная реализация НОД (алгоритм Евклида)
unsigned long gcd(unsigned long a, unsigned long b) {
    while (b != 0) {
        unsigned long temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int main() {
    setlocale(LC_ALL, "Russian");
    
    cout << "--- Задание 3: Математическая сумма ---" << endl;
    unsigned long a, b;
    cout << "Введите параметры a и b через пробел: ";
    cin >> a >> b;
    
    if (b <= 1) {
        cout << "Ряд расходится (infinity)" << endl;
    } else {
        unsigned long num = a * b;
        unsigned long den = (b - 1) * (b - 1);
        unsigned long common = gcd(num, den);
        cout << "Сумма ряда: " << num / common << "/" << den / common << endl;
    }
    
    return 0;
}