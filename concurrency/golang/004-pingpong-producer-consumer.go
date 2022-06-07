// Start n (with n between [5,10]) producers and k (with k between [5,16])
// consumers. A consumer answers random pings from producers. After answering
// 10 pings, quit the main goroutine.
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

func consumer(nt int, cases *([]reflect.SelectCase), tomain chan bool, done chan bool) {
	for {
		select {
		case <-done:
			break
		default:
			index, _, _ := reflect.Select(*cases)
			fmt.Printf("consumer f%d() pong for producer f%d()\n", nt, index+1)
			tomain <- true
		}
	}
}

func main() {
	rand.Seed(time.Now().UnixNano())

	n_producers := rand.Intn(6) + 5
	n_consumers := rand.Intn(11) + 5

	counter := 0
	limit := 10

	channels := make([]chan bool, n_producers)
	tomain := make([]chan bool, n_consumers)

	done := make(chan bool)

	cases_producers := make([]reflect.SelectCase, n_producers)
	cases_cosumers := make([]reflect.SelectCase, n_consumers)

	fmt.Println("This is main()")
	for i := range tomain {
		tomain[i] = make(chan bool)
		cases_cosumers[i] = reflect.SelectCase{
			Dir:  reflect.SelectRecv,
			Chan: reflect.ValueOf(tomain[i]),
		}
	}

	for i := range channels {
		channels[i] = make(chan bool)
		cases_producers[i] = reflect.SelectCase{
			Dir:  reflect.SelectRecv,
			Chan: reflect.ValueOf(channels[i]),
		}
	}

	for i := range tomain {
		fmt.Printf("Starting consumer f%d()\n", i+1)
		go consumer(i+1, &cases_producers, tomain[i], done)
	}

	for i := range channels {
		fmt.Printf("Starting producer f%d()\n", i+1)
		go producer(i+1, channels[i], done)
	}

	for counter < limit {
		reflect.Select(cases_cosumers)
		counter += 1
	}

	done <- true
}
