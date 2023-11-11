package main

import (
	"encoding/json"
	"fmt"
	"io/ioutil"
	"net/http"
)

// Fato representa a estrutura JSON da resposta da API
type Fato struct {
	ID        string `json:"id"`
	Texto     string `json:"text"`
	Fonte     string `json:"source"`
	URLFonte  string `json:"source_url"`
	Idioma    string `json:"language"`
	Permalink string `json:"permalink"`
}

func main() {
	// Definindo o cabeçalho de conteúdo
	fmt.Println("Content-Type: text/html; charset=utf-8")
	fmt.Println()

	fmt.Println("<html><head><title>Fato Aleatório</title></head><body>")
	fmt.Println("<h1>Fato Aleatório</h1>")

	// Chamando a API para obter um fato aleatório
	fato, err := obterFatoAleatorio()
	if err != nil {
		fmt.Println("<p>Erro ao obter o fato: ", err, "</p>")
	} else {
		fmt.Println("<p>", fato.Texto, "</p>")
		fmt.Println("<p><a href='", fato.URLFonte, "'>Fonte</a></p>")
	}

	fmt.Println("</body></html>")
}

func obterFatoAleatorio() (*Fato, error) {
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

	var fato Fato
	err = json.Unmarshal(body, &fato)
	if err != nil {
		return nil, err
	}

	return &fato, nil
}
