package main

import (
	"bufio"
	"fmt"
	"os"
)

func main() {
	in := bufio.NewReader(os.Stdin)
	out := bufio.NewWriter(os.Stdout)
	defer out.Flush()

	var n int
	fmt.Fscan(in, &n)

	for i := 0; i < n; i++ {
		var x int
		fmt.Fscan(in, &x)

		if x >= 1 && x <= 9 {
			fmt.Fprint(out, "-")
		} else {
			evenCount := 0
			for x > 0 {
				d := x % 10
				if d%2 == 0 {
					evenCount++
				}
				x /= 10
			}
			fmt.Fprint(out, evenCount)
		}

		if i != n-1 {
			fmt.Fprint(out, " ")
		}
	}
}
