package main

import (
	"fmt"
)

// VotingData хранит структуру выборов
type VotingData struct {
	Candidates []string
	VoteGroups []VoteGroup
}

type VoteGroup struct {
	Count   int
	Ranking []string
}

// SolveBorda реализует метод Борда
func SolveBorda(data VotingData) {
	n := len(data.Candidates)
	scores := make(map[string]int)

	// Инициализация баллов
	for _, cand := range data.Candidates {
		scores[cand] = 0
	}

	// Подсчет баллов
	for _, group := range data.VoteGroups {
		for i, candName := range group.Ranking {
			points := (n - 1) - i
			scores[candName] += points * group.Count
		}
	}

	fmt.Println("\n=== РЕЗУЛЬТАТЫ МЕТОДА БОРДА ===")
	winner := ""
	maxScore := -1
	for _, cand := range data.Candidates {
		score := scores[cand]
		fmt.Printf("%s: %d очков\n", cand, score)
		if score > maxScore {
			maxScore = score
			winner = cand
		}
	}
	fmt.Printf("Победитель по Борду: %s\n", winner)
}

// SolveCondorcet реализует метод Кондорсе
func SolveCondorcet(data VotingData) {
	candidates := data.Candidates
	winner := "не определен (парадокс Кондорсе)"

	fmt.Println("\n=== РЕЗУЛЬТАТЫ МЕТОДА КОНДОРСЕ ===")

	for _, candA := range candidates {
		isOverallWinner := true

		for _, candB := range candidates {
			if candA == candB {
				continue
			}

			winsA := 0
			winsB := 0

			for _, group := range data.VoteGroups {
				posA := -1
				posB := -1
				// Ищем позиции кандидатов в текущем ранжировании
				for i, name := range group.Ranking {
					if name == candA {
						posA = i
					}
					if name == candB {
						posB = i
					}
				}

				if posA < posB {
					winsA += group.Count
				} else {
					winsB += group.Count
				}
			}

			fmt.Printf("Сравнение %s vs %s: %d - %d\n", candA, candB, winsA, winsB)
			if winsA <= winsB {
				isOverallWinner = false
			}
		}

		if isOverallWinner {
			winner = candA
			break
		}
		fmt.Println("--------------------")
	}
	fmt.Printf("Победитель по Кондорсе: %s\n", winner)
}

func main() {
	var n, kGroups int

	fmt.Print("Введите количество кандидатов: ")
	fmt.Scan(&n)

	candidates := make([]string, n)
	fmt.Println("Введите имена кандидатов через пробел:")
	for i := 0; i < n; i++ {
		fmt.Scan(&candidates[i])
	}

	fmt.Print("Введите количество групп избирателей: ")
	fmt.Scan(&kGroups)

	voteGroups := []VoteGroup{}
	for g := 0; g < kGroups; g++ {
		var count int
		fmt.Printf("\nГруппа №%d. Количество избирателей: ", g+1)
		fmt.Scan(&count)

		fmt.Println("Введите их ранжирование (цепочку имен):")
		ranking := make([]string, n)
		for i := 0; i < n; i++ {
			fmt.Scan(&ranking[i])
		}
		voteGroups = append(voteGroups, VoteGroup{Count: count, Ranking: ranking})
	}

	data := VotingData{
		Candidates: candidates,
		VoteGroups: voteGroups,
	}

	SolveBorda(data)
	SolveCondorcet(data)
}
