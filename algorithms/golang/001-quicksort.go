package main

import (
	"fmt"
	"math/rand"
	"time"
)

func part(a []int) [2][]int {
	pivot := len(a) - 1
	j := 0

	for i := 0; i < len(a); i++ {
		if a[i] < a[pivot] {
			a[i], a[j] = a[j], a[i]
			j++
		}
	}

	a[j], a[pivot] = a[pivot], a[j]

	p1 := a[:j]
	p2 := a[j+1:]

	return [2][]int{p1, p2}
}

func quicksort(a []int) {
	if len(a) > 1 {
		p := part(a)
		quicksort(p[0])
		quicksort(p[1])
	}
}

func main() {
	rand.Seed(time.Now().UnixNano())

	n := rand.Intn(41) + 10
	a := make([]int, n)
	for i := 0; i < n; i++ {
		a[i] = rand.Intn(101) + 1
	}

	fmt.Println(a)
	quicksort(a)
	fmt.Println(a)
}
