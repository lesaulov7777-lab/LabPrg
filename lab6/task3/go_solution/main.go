package main

import (
	"fmt"
)

func main() {
	var choice int
	for {
		fmt.Println("=================================================")
		fmt.Println("Лабораторная работа. Задание 3 (Вариант 16) - GO")
		fmt.Println("Выберите метод решения СЛАУ:")
		fmt.Println("1. Прямой метод (Метод Гаусса с выбором главного элемента)")
		fmt.Println("2. Приближенный метод (Метод Зейделя, e = 10^-3)")
		fmt.Println("3. Запустить оба метода последовательно")
		fmt.Println("0. Выход")
		fmt.Print("Ваш выбор: ")

		_, err := fmt.Scan(&choice)
		if err != nil {
			fmt.Println("Ошибка ввода. Попробуйте еще раз.")
			continue
		}
		fmt.Println("=================================================")
		fmt.Println()

		switch choice {
		case 1:
			solveGauss()
		case 2:
			solveSeidel()
		case 3:
			solveGauss()
			fmt.Println()
			solveSeidel()
		case 0:
			fmt.Println("Завершение работы программы.")
			return
		default:
			fmt.Println("Неверный ввод. Попробуйте еще раз.")
		}
	}
}
