package main

import (
	"fmt"
	"strings"
)

// cycle проверяет, можно ли получить строку T циклическим сдвигом строки S
func cycle(S, T string) bool {
	// Если длины строк не равны, то циклический сдвиг невозможен
	if len(S) != len(T) {
		return false
	}

	// Если строки пустые, они равны
	if len(S) == 0 {
		return true
	}

	// Проверяем, является ли T подстрокой S + S
	// Если T - циклический сдвиг S, то T обязательно будет подстрокой S+S
	doubleS := S + S
	return strings.Contains(doubleS, T)
}

func main() {
	// Примеры из условия
	fmt.Println("Пример 1:")
	S1, T1 := "waterbottle", "erbottlewat"
	fmt.Printf("S = \"%s\", T = \"%s\", Результат: ", S1, T1)
	if cycle(S1, T1) {
		fmt.Println("Yes")
	} else {
		fmt.Println("No")
	}

	fmt.Println("\nПример 2:")
	S2, T2 := "abc", "acb"
	fmt.Printf("S = \"%s\", T = \"%s\", Результат: ", S2, T2)
	if cycle(S2, T2) {
		fmt.Println("Yes")
	} else {
		fmt.Println("No")
	}

	fmt.Println("\nПример 3:")
	S3, T3 := "ngtu", "ngtu"
	fmt.Printf("S = \"%s\", T = \"%s\", Результат: ", S3, T3)
	if cycle(S3, T3) {
		fmt.Println("Yes")
	} else {
		fmt.Println("No")
	}
}
