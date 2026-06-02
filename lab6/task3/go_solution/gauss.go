package main

import (
	"fmt"
	"math"
)

// Функция для красивого вывода матрицы в консоль
func printMatrix(A [][]float64, b []float64) {
	n := len(A)
	for i := 0; i < n; i++ {
		for j := 0; j < n; j++ {
			fmt.Printf("%10.4f ", A[i][j])
		}
		fmt.Printf("| %10.4f\n", b[i])
	}
	fmt.Println()
}

func solveGauss() {
	n := 4
	// Данные 16 варианта
	A := [][]float64{
		{1.08, -0.04, 0.21, -1.16},
		{0.25, -1.23, 0.22, -0.09},
		{-0.21, 0.22, 0.80, -0.13},
		{0.15, -1.31, 0.06, -1.16},
	}
	b := []float64{-1.24, -1.16, 2.56, 1.08}

	fmt.Println("=== ПРЯМОЙ МЕТОД ГАУССА С ВЫБОРОМ ГЛАВНОГО ЭЛЕМЕНТА ===")
	fmt.Println("--- Исходная расширенная матрица ---")
	printMatrix(A, b)

	// Прямой ход
	for k := 0; k < n; k++ {
		maxRow := k
		for i := k + 1; i < n; i++ {
			if math.Abs(A[i][k]) > math.Abs(A[maxRow][k]) {
				maxRow = i
			}
		}
		if maxRow != k {
			A[k], A[maxRow] = A[maxRow], A[k]
			b[k], b[maxRow] = b[maxRow], b[k]
			fmt.Printf("Перестановка строк: %d <-> %d\n", k+1, maxRow+1)
		}

		for i := k + 1; i < n; i++ {
			factor := A[i][k] / A[k][k]
			for j := k; j < n; j++ {
				A[i][j] -= factor * A[k][j]
			}
			b[i] -= factor * b[k]
		}
		fmt.Printf("Матрица после шага %d прямого хода:\n", k+1)
		printMatrix(A, b)
	}

	// Обратный ход
	x := make([]float64, n)
	for i := n - 1; i >= 0; i-- {
		sum := 0.0
		for j := i + 1; j < n; j++ {
			sum += A[i][j] * x[j]
		}
		x[i] = (b[i] - sum) / A[i][i]
	}

	fmt.Println("--- Результаты (Метод Гаусса) ---")
	for i := 0; i < n; i++ {
		fmt.Printf("x%d = %.4f\n", i+1, x[i])
	}
	fmt.Println()
}
