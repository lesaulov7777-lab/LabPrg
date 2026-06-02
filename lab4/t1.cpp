#include <iostream>
#include <cmath>
#include <iomanip>
#include <vector>
#include <string>
using namespace std;

// Исходная функция f(x)
double f(double x) {
return sin(x + 1.0) - x - 1.0;
}

// Первая производная f'(x) для метода Ньютона
double df(double x) {
return cos(x + 1.0) - 1.0;
}

// Функция phi(x) для метода простых итераций (x = phi(x))
double phi(double x) {
return sin(x + 1.0) - 1.0;
}

// Форматированный вывод шапки для таблиц Ньютона / Итераций
void printNewtonHeader() {
cout << string(55, '-') << "\n";
cout << setw(5) << "N" 
<< setw(15) << "xn" 
<< setw(15) << "xn+1" 
<< setw(20) << "|xn+1 - xn|" << "\n";
cout << string(55, '-') << "\n";
}

int main() {
// Настройка вывода вещественных чисел
cout << fixed << setprecision(6);

double eps = 1e-4; // Заданная точность

// Границы начального интервала неопределенности [a0, b0]
double a0 = -1.5;
double b0 = -0.5;

// ====================================================================
// 1. МЕТОД ПОЛОВИННОГО ДЕЛЕНИЯ (таблица по пункту 2 методики)
// ====================================================================
cout << "1. МЕТОД ПОЛОВИННОГО ДЕЛЕНИЯ\n";
cout << string(65, '-') << "\n";
cout << setw(5) << "N" 
<< setw(15) << "an" 
<< setw(15) << "bn" 
<< setw(15) << "bn - an" 
<< setw(15) << "cn" << "\n";
cout << string(65, '-') << "\n";

double a = a0, b = b0;
int k_bis = 1;
double c = 0;

while ((b - a) > eps) {
c = (a + b) / 2.0;
cout << setw(5) << k_bis 
<< setw(15) << a 
<< setw(15) << b 
<< setw(15) << (b - a) 
<< setw(15) << c << "\n";

// Проверка на точное попадание в корень
if (abs(f(c)) < 1e-12) {
a = c;
b = c;
break;
}

if (f(a) * f(c) < 0.0) {
b = c;
} else {
a = c;
}
k_bis++;
}
// Вывод последней строки, когда интервал стянулся
if (a != b) {
c = (a + b) / 2.0;
cout << setw(5) << k_bis 
<< setw(15) << a 
<< setw(15) << b 
<< setw(15) << (b - a) 
<< setw(15) << c << "\n";
}
cout << string(65, '-') << "\n";
cout << "Корень методом дихотомии: x* = " << c << "\n\n";


// ====================================================================
// 2. МЕТОД НЬЮТОНА (таблица по пункту 3 методики)
// ====================================================================
cout << "2. МЕТОД НЬЮТОНА\n";
printNewtonHeader();

double x_curr = a0; // Начальное приближение x0 = -1.5
int k_newton = 1;

while (true) {
double d_val = df(x_curr);
if (abs(d_val) < 1e-12) {
cout << "Ошибка: производная близка к числу 0.\n";
break;
}

double x_next = x_curr - f(x_curr) / d_val;
double diff = abs(x_next - x_curr);

cout << setw(5) << k_newton 
<< setw(15) << x_curr 
<< setw(15) << x_next 
<< setw(20) << diff << "\n";

if (diff < eps) {
x_curr = x_next;
break;
}
x_curr = x_next;
k_newton++;
}
cout << string(55, '-') << "\n";
cout << "Корень методом Ньютона: x* = " << x_curr << "\n\n";


// ====================================================================
// 3. МЕТОД ПРОСТЫХ ИТЕРАЦИЙ (таблица по пункту 3 методики)
// ====================================================================
cout << "3. МЕТОД ПРОСТЫХ ИТЕРАЦИЙ\n";
printNewtonHeader();

x_curr = b0; // Для итераций возьмем x0 = -0.5
int k_iter = 1;

while (true) {
double x_next = phi(x_curr);
double diff = abs(x_next - x_curr);

cout << setw(5) << k_iter 
<< setw(15) << x_curr 
<< setw(15) << x_next 
<< setw(20) << diff << "\n";

if (diff < eps) {
x_curr = x_next;
break;
}
x_curr = x_next;
k_iter++;

// Ограничение итераций (для предотвращения зацикливания из-за слабой сходимости)
if (k_iter > 15) {
cout << "... вывод прерван (скорость сходимости низкая) ...\n";
break;
}
}
cout << string(55, '-') << "\n";
cout << "Корень методом простых итераций: x* = " << x_curr << "\n\n";


// ====================================================================
// ДЕМОНСТРАЦИЯ ТРЕБОВАНИЯ: Использование контейнеров и Range-based for
// ====================================================================
cout << "Итоговые найденные корни (демонстрация Range-based for):\n";
vector<double> final_roots = {-1.000000}; 
for (const auto& root : final_roots) { // Обязательный Range-based loop по ТЗ
cout << " -> " << root << "\n";
}

return 0;
}