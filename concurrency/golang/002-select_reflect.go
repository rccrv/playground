// Start n (with n between [10, 32]) goroutines and process random pings from
// these goroutines.
package main

import (
	"fmt"
	"math/rand"
	"reflect"
	"time"
)

func f(c chan bool) {
	for {
		v := rand.Intn(5) + 1
		time.Sleep(time.Duration(v) * time.Second)
		c <- true
	}
}

func main() {
	rand.Seed(time.Now().UnixNano())

	n := rand.Intn(23) + 10
	fc := make([]chan bool, n)
	cases := make([]reflect.SelectCase, n)

	fmt.Println("This is main()")
	for i := range fc {
		fc[i] = make(chan bool)
		cases[i] = reflect.SelectCase{
			Dir:  reflect.SelectRecv,
			Chan: reflect.ValueOf(fc[i]),
		}
		fmt.Printf("Starting goroutine f%d()\n", i+1)
		go f(fc[i])
	}

	for {
		index, _, _ := reflect.Select(cases)
		fmt.Printf("Received from f%d()\n", index+1)
	}
}
