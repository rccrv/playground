package main

import (
	"fmt"
	"math/rand"
	"reflect"
	"time"
)

func producer(nt int, sender chan bool, done chan bool) {
	for {
		select {
		case <-done:
			break
		default:
			v := rand.Intn(5) + 1
			time.Sleep(time.Duration(v) * time.Second)

			if rand.Float32() > 0.7 {
				fmt.Printf("producer f%d() ping\n", nt)
				sender <- true
			}
		}
	}
}

func consumer(nt int, receiver chan bool, tomain chan bool, done chan bool) {
	for {
		select {
		case <-receiver:
			fmt.Printf("consumer f%d() pong\n", nt)
			tomain <- true
		case <-done:
			break
		}
	}
}

func main() {
	rand.Seed(time.Now().UnixNano())

	n := rand.Intn(12) + 5

	counter := 0
	limit := 10

	channels := make([]chan bool, n)
	tomain := make([]chan bool, n)

	done := make(chan bool)

	cases := make([]reflect.SelectCase, n)

	fmt.Println("This is main()")
	for i := range channels {
		channels[i] = make(chan bool)
		tomain[i] = make(chan bool)

		cases[i] = reflect.SelectCase{
			Dir:  reflect.SelectRecv,
			Chan: reflect.ValueOf(tomain[i]),
		}

		fmt.Printf("Starting producer f%d()\n", i+1)
		go producer(i+1, channels[i], done)

		fmt.Printf("Starting consumer f%d()\n", i+1)
		go consumer(i+1, channels[i], tomain[i], done)
	}

	for counter < limit {
		reflect.Select(cases)
		counter += 1
	}

	done <- true
}
