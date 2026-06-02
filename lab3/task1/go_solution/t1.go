package main

import (
	"fmt"
	"math"
)

// Задание 1: Таблица значений функции
func main() {
	fmt.Println("--- Задание 1: Таблица значений функции ---")
	dx := 0.5 // Шаг таблицы
	fmt.Printf("%10s %15s\n", "x", "y")
	fmt.Println("--------------------------")

	// Цикл перебора аргумента от -6.0 до 8.0
	// Добавляется dx/2 для компенсации погрешности чисел с плавающей точкой
	for x := -6.0; x <= 8.0+dx/2; x += dx {
		var y float64

		// Проверка интервалов согласно графику функции
		if x >= -6 && x <= -2 {
			y = -math.Sqrt(-(x + 2))
		} else if x > -2 && x <= -1 {
			y = math.Sqrt(1 - math.Pow(x+1, 2))
		} else if x > -1 && x <= 0 {
			y = x * x
		} else if x > 0 && x <= 2 {
			y = 0.5 * x * x
		} else if x > 2 && x <= 4 {
			y = math.Sqrt(4 - math.Pow(x-2, 2))
		} else if x > 4 && x <= 8 {
			y = -math.Sqrt(x - 4)
		} else {
			continue
		}

		// Вывод строки таблицы с фиксированной точностью (2 знака после запятой)
		fmt.Printf("%10.2f %15.2f\n", x, y)
	}
}
