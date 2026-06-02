#ifndef GAUSS_HPP
#define GAUSS_HPP

#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

void printMatrix(const std::vector<std::vector<double>>& A, const std::vector<double>& b) {
    int n = A.size();
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            std::cout << std::setw(10) << std::fixed << std::setprecision(4) << A[i][j] << " ";
        }
        std::cout << "| " << std::setw(10) << b[i] << std::endl;
    }
    std::cout << std::endl;
}

void solveGauss() {
    int n = 4;
    // Данные 16 варианта
    std::vector<std::vector<double>> A = {
        { 1.08, -0.04,  0.21, -1.16},
        { 0.25, -1.23,  0.22, -0.09},
        {-0.21,  0.22,  0.80, -0.13},
        { 0.15, -1.31,  0.06, -1.16}
    };
    std::vector<double> b = {-1.24, -1.16, 2.56, 1.08};

    std::cout << "=== ПРЯМОЙ МЕТОД ГАУССА С ВЫБОРОМ ГЛАВНОГО ЭЛЕМЕНТА ===" << std::endl;
    std::cout << "--- Исходная расширенная матрица ---" << std::endl;
    printMatrix(A, b);

    // Прямой ход
    for (int k = 0; k < n; ++k) {
        int maxRow = k;
        for (int i = k + 1; i < n; ++i) {
            if (std::abs(A[i][k]) > std::abs(A[maxRow][k])) {
                maxRow = i;
            }
        }
        if (maxRow != k) {
            std::swap(A[k], A[maxRow]);
            std::swap(b[k], b[maxRow]);
            std::cout << "Перестановка строк: " << k + 1 << " <-> " << maxRow + 1 << std::endl;
        }

        for (int i = k + 1; i < n; ++i) {
            double factor = A[i][k] / A[k][k];
            for (int j = k; j < n; ++j) {
                A[i][j] -= factor * A[k][j];
            }
            b[i] -= factor * b[k];
        }
        std::cout << "Матрица после шага " << k + 1 << " прямого хода:" << std::endl;
        printMatrix(A, b);
    }

    // Обратный ход
    std::vector<double> x(n);
    for (int i = n - 1; i >= 0; --i) {
        double sum = 0;
        for (int j = i + 1; j < n; ++j) {
            sum += A[i][j] * x[j];
        }
        x[i] = (b[i] - sum) / A[i][i];
    }

    std::cout << "--- Результаты (Метод Гаусса) ---" << std::endl;
    for (int i = 0; i < n; ++i) {
        std::cout << "x" << i + 1 << " = " << std::fixed << std::setprecision(4) << x[i] << std::endl;
    }
    std::cout << std::endl;
}

#endif