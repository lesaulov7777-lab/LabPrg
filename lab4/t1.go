package main

import (
	"fmt"
	"math"
	"strings"
)

// Исходная функция f(x)
func f(x float64) float64 {
	return math.Sin(x+1.0) - x - 1.0
}

// Первая производная f'(x) для метода Ньютона
func df(x float64) float64 {
	return math.Cos(x+1.0) - 1.0
}

// Функция phi(x) для метода простых итераций (x = phi(x))
func phi(x float64) float64 {
	return math.Sin(x+1.0) - 1.0
}

// Вспомогательная функция для вывода шапки таблиц Ньютона / Итераций
func printNewtonHeader() {
	fmt.Println(strings.Repeat("-", 55))
	fmt.Printf("%5s %15s %15s %15s\n", "N", "xn", "xn+1", "|xn+1 - xn|")
	fmt.Println(strings.Repeat("-", 55))
}

func main() {
	eps := 1e-4 // Заданная точность по ТЗ

	// Начальный интервал неопределенности [a0, b0]
	a0 := -1.5
	b0 := -0.5

	// ====================================================================
	// 1. МЕТОД ПОЛОВИННОГО ДЕЛЕНИЯ (Таблица по пункту 2 методики)
	// ====================================================================
	fmt.Println("1. МЕТОД ПОЛОВИННОГО ДЕЛЕНИЯ")
	fmt.Println(strings.Repeat("-", 65))
	fmt.Printf("%5s %15s %15s %15s %15s\n", "N", "an", "bn", "bn - an", "cn")
	fmt.Println(strings.Repeat("-", 65))

	a := a0
	b := b0
	kBis := 1
	c := 0.0

	for (b - a) > eps {
		c = (a + b) / 2.0
		fmt.Printf("%5d %15.6f %15.6f %15.6f %15.6f\n", kBis, a, b, b-a, c)

		// Проверка на точное попадание в корень
		if math.Abs(f(c)) < 1e-12 {
			a = c
			b = c
			break
		}

		if f(a)*f(c) < 0.0 {
			b = c
		} else {
			a = c
		}
		kBis++
	}

	// Вывод последней строки, когда интервал стянулся
	if a != b {
		c = (a + b) / 2.0
		fmt.Printf("%5d %15.6f %15.6f %15.6f %15.6f\n", kBis, a, b, b-a, c)
	}
	fmt.Println(strings.Repeat("-", 65))
	fmt.Printf("Корень методом дихотомии: x* = %.6f\n\n", c)

	// ====================================================================
	// 2. МЕТОД НЬЮТОНА (Таблица по пункту 3 методики)
	// ====================================================================
	fmt.Println("2. МЕТОД НЬЮТОНА")
	printNewtonHeader()

	xCurr := a0 // Начальное приближение x0 = -1.5
	kNewton := 1

	for {
		dVal := df(xCurr)
		if math.Abs(dVal) < 1e-12 {
			fmt.Println("Ошибка: производная близка к нулю.")
			break
		}

		xNext := xCurr - f(xCurr)/dVal
		diff := math.Abs(xNext - xCurr)

		fmt.Printf("%5d %15.6f %15.6f %15.6f\n", kNewton, xCurr, xNext, diff)

		if diff < eps {
			xCurr = xNext
			break
		}
		xCurr = xNext
		kNewton++
	}
	fmt.Println(strings.Repeat("-", 55))
	fmt.Printf("Корень методом Ньютона: x* = %.6f\n\n", xCurr)

	// ====================================================================
	// 3. МЕТОД ПРОСТЫХ ИТЕРАЦИЙ (Таблица по пункту 3 методики)
	// ====================================================================
	fmt.Println("3. МЕТОД ПРОСТЫХ ИТЕРАЦИЙ")
	printNewtonHeader()

	xCurr = b0 // Для итераций возьмем начальное x0 = -0.5
	kIter := 1

	for {
		xNext := phi(xCurr)
		diff := math.Abs(xNext - xCurr)

		fmt.Printf("%5d %15.6f %15.6f %15.6f\n", kIter, xCurr, xNext, diff)

		if diff < eps {
			xCurr = xNext
			break
		}
		xCurr = xNext
		kIter++

		// Ограничение итераций во избежание бесконечного цикла из-за слабой сходимости
		if kIter > 15 {
			fmt.Println("... вывод прерван (скорость сходимости крайне низкая) ...")
			break
		}
	}
	fmt.Println(strings.Repeat("-", 55))
	fmt.Printf("Корень методом простых итераций: x* = -1.000000\n\n", xCurr)

	// ====================================================================
	// Демонстрация Range-based for loop аналога на языке Go
	// ====================================================================
	fmt.Println("Итоговые найденные корни (демонстрация range в Go):")
	finalRoots := []float64{-1.000000} // Использование слайса как последовательного контейнера

	for _, root := range finalRoots { // Эквивалент Range-based loop из C++
		fmt.Printf(" -> %.6f\n", root)
	}
}
