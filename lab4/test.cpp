#include <iostream>
#include <cmath>
#include <vector>  

using namespace std;

const double EPS = 1e-4;

double f(double x) {
    return sin(x + 1) - 0.2 * x;
}

double df(double x) {
    return cos(x + 1) - 0.2;
}

void bisection(double a, double b) {
    cout << "Метод половинного деления" << endl << endl;
    int n = 0;
    double c;
     
    while ((b - a) > EPS) {
        c = (a + b) / 2.0;
        cout << "N = " << n << "\t a = " << a << " \t b = " << b << " \t b - a = " << b - a << endl;

        if (f(a) * f(c) <= 0) 
            b = c; 
        else 
            a = c;
        n++;
    }

    cout << "Корень: " << (a + b) / 2.0 << " итераций: " << n << "" << endl << endl;
}


double newton(double x0) {
    cout << "Метод Ньютона" << endl << endl;
    int n = 0;
    double x = x0;
    double x_new;      
    double delta = 1.0; 

    while (delta > EPS) {
        double fx = f(x);
        double dfx = df(x);

        x_new = x - fx / dfx;

        delta = fabs(x_new - x); 

        cout << "N = " << n << "\t x_n = " << x << "\t x_{n+1} = " << x_new << "\t x_{n+1}-x_n" << delta << endl;

        x = x_new;
        n++;
    }

    cout << "Корень: " << x << "  итераций: " << n << endl << endl;
    return (x);
}

void chord(double a, double b) {
    cout << "Метод хорд" << endl << endl;
    int n = 0;
    double x, x_new = 0;

    x = a;

    while (fabs(f(x_new)) > EPS) {
        
        x_new = a - f(a) * (b - a) / (f(b) - f(a));
        double delta = fabs(x_new - x);

        cout << "N=" << n << " a=" << a << " b=" << b << " x=" << x_new << " |diff|=" << delta << "\n";

        
        if (f(a) * f(x_new) <= 0) 
            b = x_new;
        else 
            a = x_new;

        x = x_new;
        n++;

    } 

    cout << "Корень: " << x_new << " итераций: " << n << endl << endl;
}

int main() {
    setlocale(LC_ALL, "Russian");
    vector<double> x;
    cout << "Решение уравнения sin(x+1) = 0.2x" << endl << endl;


    vector<pair<double, double>> segments = {
        {-4.0, -3.0}, 
        {-2.0, -1.0},  
        {1.0, 2.0}     
    };

    for (auto seg: segments) {
        double a = seg.first;
        double b = seg.second;

        cout << "ОТРЕЗОК [" << a << ", " << b << "]" << endl << endl;


        bisection(a, b);
        x.push_back(newton((a + b) / 2.0));
        chord(a, b);  
    }

    int i = 1;
    for (auto root : x) {
        cout << "x" << i << " = " << root << endl;
        i++;
    } 

    return 0;
}
