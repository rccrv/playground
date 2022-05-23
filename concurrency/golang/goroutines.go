package main

import (
	"fmt"
	"math/rand"
	"time"
)

func f(c chan bool, fn int) {
	fmt.Println("This is f() number ", fn)
	for {
		v := rand.Intn(5) + 1
		time.Sleep(time.Duration(v) * time.Second)
		c <- true
	}
}

func main() {
	rand.Seed(time.Now().UnixNano())
	f1c := make(chan bool)
	f2c := make(chan bool)
	f3c := make(chan bool)
	go f(f1c, 1)
	go f(f2c, 2)
	go f(f3c, 3)
	fmt.Println("This is main()")
	for {
		select {
		case <-f1c:
			fmt.Println("Received from f() number 1")
		case <-f2c:
			fmt.Println("Received from f() number 2")
		case <-f3c:
			fmt.Println("Received from f() number 3")
		}
	}
}
