package main

import (
	"fmt"
)

// Задание 4: Игра с числами
func main() {
	fmt.Println("--- Задание 4: Игра с числами ---")
	// Структура для ключа карты, описывающая состояние игры
	type State struct {
		Rem  int // Сколько чисел осталось на столе
		Last int // Сколько чисел текущий игрок взял на своем прошлом ходу
	}

	var n, m int
	fmt.Print("Введите количество чисел (N) и ограничение на ход (M) через пробел: ")
	_, err := fmt.Scan(&n, &m)
	if err != nil {
		fmt.Println("Ошибка ввода параметров!")
		return
	}

	// Считываем сам массив чисел
	numbers := make([]int, n)
	fmt.Printf("Введите %d чисел(ла) через пробел или Enter: ", n)
	for i := 0; i < n; i++ {
		fmt.Scan(&numbers[i])
	}

	// Карта для сохранения вычисленных состояний (аналог таблицы memo)
	memo := make(map[State]bool)

	// Итеративный просчет дерева ходов снизу вверх (от 0 до N оставшихся чисел)
	for rem := 0; rem <= n; rem++ {
		for last := 0; last <= m; last++ {

			// Базовый случай: если чисел нет, текущий игрок проиграл
			if rem == 0 {
				memo[State{Rem: rem, Last: last}] = false
				continue
			}

			currentStateWin := false

			// Перебираем все варианты текущего хода (сколько чисел стереть)
			for take := 1; take <= m; take++ {
				// Проверяем правила: берем не больше, чем есть, и не повторяем свой прошлый ход
				if take <= rem && take != last {

					// Состояние, в которое попадет соперник
					nextState := State{Rem: rem - take, Last: take}

					// Если соперник из того состояния гарантированно проигрывает,
					// значит наш текущий ход — выигрышный
					if nextWin, exists := memo[nextState]; exists && !nextWin {
						currentStateWin = true
						break // Нашли победный ход, остальные варианты можно не перебирать
					}
				}
			}

			// Сохраняем результат для этой комбинации параметров
			memo[State{Rem: rem, Last: last}] = currentStateWin
		}
	}

	fmt.Println("---------------------------------------")
	fmt.Print("Результат игры (1 - Павел, 0 - Вика): ")

	// Проверяем финальное состояние игры для N элементов.
	// У Павла на первом ходу ограничений нет, поэтому Last = 0.
	initialState := State{Rem: n, Last: 0}
	if memo[initialState] {
		fmt.Println("1 (Выиграл Павел)")
	} else {
		fmt.Println("0 (Выиграла Вика)")
	}
}
