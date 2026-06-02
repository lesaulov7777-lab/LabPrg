package main

import (
	"fmt"
	"math"
)

func solveSeidel() {
	eps := 1e-3 // Точность

	// Коэффициенты канонической матрицы C и вектора f для Варианта 16
	C := [][]float64{
		{0.0, -0.0339, -0.3136, 0.0763},
		{0.2033, 0.0, 0.1789, -0.0732},
		{0.2625, -0.2750, 0.0, 0.1625},
		{-0.1090, 0.0, -0.1635, 0.0},
	}
	f := []float64{-2.9492, 0.9431, 3.2000, -2.1761}

	x := []float64{0.0, 0.0, 0.0, 0.0} // Начальное приближение
	xOld := make([]float64, 4)

	fmt.Println("=== ПРИБЛИЖЕННЫЙ МЕТОД ИТЕРАЦИЙ (ЗЕЙДЕЛЯ) ===")
	fmt.Printf("%-3s %-11s %-11s %-11s %-11s %-14s\n", "N", "x1", "x2", "x3", "x4", "eps_n")
	fmt.Println("------------------------------------------------------------------------")

	iteration := 0
	maxDiff := 1.0

	for maxDiff > eps && iteration < 100 {
		iteration++
		copy(xOld, x)

		for i := 0; i < 4; i++ {
			sum := f[i]
			for j := 0; j < 4; j++ {
				sum += C[i][j] * x[j] // В Go слайсы передаются по ссылке, берем обновленные x
			}
			x[i] = sum
		}

		// Оценка погрешности текущей итерации
		maxDiff = 0.0
		for i := 0; i < 4; i++ {
			diff := math.Abs(x[i] - xOld[i])
			if diff > maxDiff {
				maxDiff = diff
			}
		}

		fmt.Printf("%-3d %-11.4f %-11.4f %-11.4f %-11.4f %-14.2e\n",
			iteration, x[0], x[1], x[2], x[3], maxDiff)
	}

	fmt.Println("\n--- Финальные результаты (Метод Зейделя) ---")
	for i := 0; i < 4; i++ {
		fmt.Printf("x%d = %.4f\n", i+1, x[i])
	}
	fmt.Println()
}
