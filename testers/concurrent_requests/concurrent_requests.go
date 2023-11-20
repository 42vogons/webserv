package main

import (
	"fmt"
	"net/http"
	"os"
	"strconv"
	"sync"
)

func main() {
	if len(os.Args) != 3 {
		fmt.Println("Usage: ./concurrent_requests.go [concurrentRequests] [serverURL]")
		fmt.Println("Example: ./concurrent_requests.go 999 http://localhost:8080")
		os.Exit(1)
	}

	concurrentRequests, err := strconv.Atoi(os.Args[1])
	if err != nil {
		fmt.Printf("Invalid number for concurrent requests: %s\n", err)
		fmt.Println("Usage: ./concurrent_requests.go [concurrentRequests] [serverURL]")
		fmt.Println("Example: ./concurrent_requests.go 999 http://localhost:8080")
		os.Exit(1)
	}

	serverURL := os.Args[2]

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
