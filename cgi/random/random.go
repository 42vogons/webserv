package main

import (
	"encoding/json"
	"fmt"
	"io/ioutil"
	"net/http"
)

type Source struct {
	ID        string `json:"id"`
	Text      string `json:"text"`
	Source    string `json:"source"`
	SourceURL string `json:"source_url"`
	Language  string `json:"language"`
	Permalink string `json:"permalink"`
}

func main() {
	fmt.Println("Content-Type: text/html; charset=utf-8")
	fmt.Println()
	fmt.Println("<html><head><title>Useless Fact</title></head><body>")
	fmt.Println("<h1>Useless Fact</h1>")
	source, err := getSource()
	if err != nil {
		fmt.Println("<p>error getting useless fact: ",err,"</p>")
	} else {
		fmt.Println("<p>",  source.Text,"</p>")
		fmt.Println("<p><a href='", source.SourceURL,"'>Font</a></p>")
	}
	fmt.Println("</body></html>")
}

func getSource() (*Source, error) {
	url := "https://uselessfacts.jsph.pl/api/v2/facts/random?language=%22pt-BR%22"
	res, err := http.Get(url)
	if err != nil {
		return nil, err
	}
	defer res.Body.Close()
	body, err := ioutil.ReadAll(res.Body)
	if err != nil {
		return nil, err
	}
	var source Source
	err = json.Unmarshal(body, &source)
	if err != nil {
		return nil, err
	}
	return &source, nil
}
