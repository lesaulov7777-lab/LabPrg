package main

import (
	"fmt"
	"os"
)

var N, M int

// Функция для красивого вывода маски в виде доски в терминал
func printMatrix(mask int) {
	for i := 0; i < N; i++ {
		fmt.Print("  ")
		for j := 0; j < M; j++ {
			bit := (mask >> (i*M + j)) & 1
			fmt.Printf("%d ", bit)
		}
		fmt.Println()
	}
}

// Генерация целевых состояний "Рамки"
func getTargetMasks() (int, int) {
	maskA := 0 // По краю 1, внутри 0
	maskB := 0 // По краю 0, внутри 1

	for i := 0; i < N; i++ {
		for j := 0; j < M; j++ {
			bitPos := i*M + j
			isBorder := (i == 0 || i == N-1 || j == 0 || j == M-1)

			if isBorder {
				maskA |= (1 << bitPos)
			} else {
				maskB |= (1 << bitPos)
			}
		}
	}
	return maskA, maskB
}

func solve(startMask int) int {
	targetA, targetB := getTargetMasks()

	// Если стартовое состояние уже является рамкой
	if startMask == targetA || startMask == targetB {
		return 0
	}

	// Очередь для BFS (используем slice)
	queue := []int{startMask}

	// Карта расстояний (хранит {состояние : количество_ходов})
	dist := make(map[int]int)
	dist[startMask] = 0

	for len(queue) > 0 {
		// Достаем элемент из начала очереди
		curr := queue[0]
		queue = queue[1:]

		currDist := dist[curr]

		// Перебираем все пары соседних клеток
		for i := 0; i < N; i++ {
			for j := 0; j < M; j++ {
				p1 := i*M + j

				// Сосед справа
				if j+1 < M {
					p2 := i*M + (j + 1)
					nextMask := curr ^ (1 << p1) ^ (1 << p2)

					if nextMask == targetA || nextMask == targetB {
						return currDist + 1
					}
					if _, visited := dist[nextMask]; !visited {
						dist[nextMask] = currDist + 1
						queue = append(queue, nextMask)
					}
				}

				// Сосед снизу
				if i+1 < N {
					p2 := (i+1)*M + j
					nextMask := curr ^ (1 << p1) ^ (1 << p2)

					if nextMask == targetA || nextMask == targetB {
						return currDist + 1
					}
					if _, visited := dist[nextMask]; !visited {
						dist[nextMask] = currDist + 1
						queue = append(queue, nextMask)
					}
				}
			}
		}
	}
	return -1
}

func main() {
	fmt.Println("=========================================================")
	fmt.Println("   ПРОГРАММА ПОИСКА МИНИМАЛЬНОГО ПУТИ К РАМКЕ МОНЕТ (GO)")
	fmt.Println("=========================================================")
	fmt.Println()

	fmt.Println("[Шаг 1] Введите размеры щита (N - строки, M - столбцы):")
	fmt.Print("Формат ввода: N M (например, 3 3): ")

	_, err := fmt.Scan(&N, &M)
	if err != nil || N <= 0 || M <= 0 {
		fmt.Println("\n[Ошибка] Некорректный размер доски!")
		os.Exit(1)
	}

	// Проверка ограничений для битовой маски (32-битный int)
	if N*M > 30 {
		fmt.Println("\n[Предупреждение] Размер доски слишком велик для BFS перебора масок (> 30 клеток).")
		os.Exit(1)
	}

	// Показываем пользователю целевые конфигурации
	tA, tB := getTargetMasks()
	fmt.Printf("\n[Инфо] Целевые состояния («Рамки») для размера %dx%d:\n", N, M)
	fmt.Println("Вариант А (1 по краям):")
	printMatrix(tA)
	fmt.Println("Вариант Б (0 по краям):")
	printMatrix(tB)
	fmt.Println("---------------------------------------------------------")
	fmt.Println()

	fmt.Println("[Шаг 2] Введите матрицу монет (строка за строкой).")
	fmt.Println("Используйте '0' для орла и '1' для решки:")

	startMask := 0
	for i := 0; i < N; i++ {
		fmt.Printf("Строка %d (введите %d чисел через пробел): ", i+1, M)
		for j := 0; j < M; j++ {
			var val int
			fmt.Scan(&val)
			if val != 0 && val != 1 {
				fmt.Println("\n[Ошибка] Разрешены только значения 0 или 1!")
				os.Exit(1)
			}
			if val == 1 {
				startMask |= (1 << (i*M + j))
			}
		}
	}

	fmt.Println("\n---------------------------------------------------------")
	fmt.Println("[Статус] Исходная доска принята. Запускаю расчет алгоритма BFS...")
	fmt.Println("---------------------------------------------------------")
	fmt.Println()

	result := solve(startMask)

	fmt.Println("==================== РЕЗУЛЬТАТ ==========================")
	if result == -1 {
		fmt.Println(" Исходное состояние: ")
		printMatrix(startMask)
		fmt.Println("\n ПРИГОВОР: Из данного расположения получить рамку НЕВОЗМОЖНО.")
	} else {
		fmt.Println(" УСПЕХ! Целевой рисунок достижим.")
		fmt.Printf(" Минимальное количество ходов (переворотов пар): %d\n", result)
	}
	fmt.Println("=========================================================")
}
