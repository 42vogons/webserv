package main

import (
	"fmt"
	"net/http"
	"sync"
)

const (
	concurrentRequests = 999
	serverURL          = "http://localhost:8080"
)

func main() {
	var wg sync.WaitGroup

	for i := 0; i < concurrentRequests; i++ {
		wg.Add(1)

		go func(id int) {
			defer wg.Done()

			resp, err := http.Get(serverURL)
			if err != nil {
				fmt.Printf("Request %d failed: %s\n", id, err)
				return
			}
			defer resp.Body.Close()

			fmt.Printf("Request %d completed with status: %s\n", id, resp.Status)
		}(i)
	}

	wg.Wait()
	fmt.Println("All requests completed!")
}
