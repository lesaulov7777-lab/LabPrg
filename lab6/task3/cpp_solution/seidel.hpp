#ifndef SEIDEL_HPP
#define SEIDEL_HPP

#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

void solveSeidel() {
    double eps = 1e-3; // Точность
    
    // Коэффициенты канонической матрицы C и вектора f для Варианта 16
    std::vector<std::vector<double>> C = {
        {0.0,       -0.0339, -0.3136,  0.0763},
        {0.2033,     0.0,     0.1789, -0.0732},
        {0.2625,    -0.2750,  0.0,     0.1625},
        {-0.1090,    0.0,    -0.1635,  0.0}
    };
    std::vector<double> f = {-2.9492, 0.9431, 3.2000, -2.1761};
    
    std::vector<double> x = {0.0, 0.0, 0.0, 0.0}; // Начальное приближение
    std::vector<double> x_old = x;
    
    std::cout << "=== ПРИБЛИЖЕННЫЙ МЕТОД ИТЕРАЦИЙ (ЗЕЙДЕЛЯ) ===" << std::endl;
    std::cout << "N" << std::setw(12) << "x1" << std::setw(12) << "x2" << std::setw(12) << "x3" << std::setw(12) << "x4" << std::setw(15) << "eps_n" << std::endl;
    std::cout << "------------------------------------------------------------------------" << std::endl;
    
    int iteration = 0;
    double max_diff = 1.0;
    
    while (max_diff > eps && iteration < 100) {
        iteration++;
        x_old = x;
        
        for (int i = 0; i < 4; ++i) {
            double sum = f[i];
            for (int j = 0; j < 4; ++j) {
                sum += C[i][j] * x[j];
            }
            x[i] = sum;
        }
        
        // Оценка погрешности текущей итерации
        max_diff = 0.0;
        for (int i = 0; i < 4; ++i) {
            if (std::abs(x[i] - x_old[i]) > max_diff) {
                max_diff = std::abs(x[i] - x_old[i]);
            }
        }
        
        std::cout << iteration << std::setw(12) << std::fixed << std::setprecision(4) << x[0] 
                  << std::setw(12) << x[1] << std::setw(12) << x[2] << std::setw(12) << x[3] 
                  << std::setw(15) << std::scientific << std::setprecision(2) << max_diff << std::endl;
    }
    
    std::cout << "\n--- Финальные результаты (Метод Зейделя) ---" << std::endl;
    for (int i = 0; i < 4; ++i) {
        std::cout << "x" << i + 1 << " = " << std::fixed << std::setprecision(4) << x[i] << std::endl;
    }
    std::cout << std::endl;
}

#endif