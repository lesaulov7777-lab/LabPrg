package main

import (
	"fmt"
)

// Собственная реализация НОД (алгоритм Евклида) для unsigned long (uint64)
func gcd(a, b uint64) uint64 {
	for b != 0 {
		temp := b
		b = a % b
		a = temp
	}
	return a
}

// Задание 3: Математическая сумма
func main() {
	fmt.Println("--- Задание 3: Математическая сумма ---")
	var a, b uint64
	fmt.Print("Введите параметры a и b через пробел: ")
	_, err := fmt.Scan(&a, &b)
	if err != nil {
		fmt.Println("Ошибка ввода")
		return
	}

	// Если знаменатель прогрессии b <= 1, то бесконечный ряд расходится
	if b <= 1 {
		fmt.Println("Ряд расходится (infinity)")
	} else {
		// Вычисление числителя и знаменателя по формуле: S = (a * b) / (b - 1)^2
		num := a * b
		den := (b - 1) * (b - 1)

		// Сокращение полученной дроби с помощью функции НОД
		common := gcd(num, den)
		fmt.Printf("Сумма ряда: %d/%d\n", num/common, den/common)
	}
}
