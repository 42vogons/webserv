#!/bin/bash

webserver="../webserv"
config_file="conf_test"
PORT1=8080
PORT2=8083
SERVER1=http://www.site1.com
SERVER2=http://www.site2.com



cp -r ../pages .
cp -r ../images .
cp -r ../cgi .

cp -r image_small.png pages/site2/uploads

# Inicie o servidor web em segundo plano com a configuração especificada
"$webserver" "$config_file" &

# Aguarde um tempo para garantir que o servidor web esteja totalmente iniciado
sleep 2


# Variáveis para contar os acertos e erros
acertos=0
erros=0

# Função para verificar o código de status da resposta
check_status() {
	method=$1
	expected_status=$2
	url=$3

	if [ "$method" = "GET" ]; then
		http_status=$(curl -s -o /dev/null -w "%{http_code}" "$url")
	fi

	if [ "$method" = "POST_FILE" ]; then
		http_status=$(curl -s -o /dev/null -X POST -w "%{http_code}" -F "imagem=@$4" "$url")
	fi

	if [ "$method" = "POST" ]; then
		http_status=$(curl -s -o -X POST -d "$4" "$url")
	fi

	if [ "$method" = "DELETE" ]; then
		http_status=$(curl -s -o /dev/null -X DELETE -w "%{http_code}" "$url")
	fi

	if [ "$http_status" = "$expected_status" ]; then
		echo -e "\e[32mOK (código de status $http_status)\e[0m"
		((acertos++))
	else
		echo -e "\e[31mFAIL (código de status $http_status)\e[0m"
		((erros++))
	fi
	echo ""
}

# Teste 1: GET de uma página válida
echo "TEST 1 - GET VALID PAGE EXPECTED 200"
check_status "GET" 200 "$SERVER1:$PORT1/"

# Teste 2: GET de uma página inválida
echo "TEST 2 - GET INVALID PAGE EXPECTED 404"
check_status "GET" 404 "$SERVER1:$PORT1/pages/lalala.html"

# Teste 3: GET de uma imagem inválida
echo "TEST 3 - GET INVALID PAGE IMAGE EXPECTED 404"
check_status "GET" 404 "$SERVE1:$PORT1/pages/site1/uploads/image_small.png"

# Teste 4: POST envio de arquivo (redirecionamento)
echo "TEST 4 - POST SEND IMAGE EXPECTED 301 - REDIRECT"
check_status "POST_FILE" 301 "$SERVER1:$PORT1/pages/site1/upload.html" "image_small.png"

# Teste 5: GET de uma imagem válida
echo "TEST 5 - GET VALID PAGE IMAGE EXPECTED 200"
check_status "GET" 200 "$SERVER1:$PORT1/pages/site1/uploads/image_small.png"

# Teste 6: DELETE de um arquivo enviado
echo "TEST 6 - DELETE FILE EXPECTED 200"
check_status "DELETE" 200 "$SERVER1:$PORT1/pages/site1/uploads/image_small.png"

# Teste 7: DELETE de um arquivo já deletado
echo "TEST 7 - DELETE INVALID FILE EXPECTED 404"
check_status "DELETE" 404 "$SERVER1:$PORT1/pages/site1/uploads/image_small.png"

# Teste 8: GET de uma página não encontrada mas autoindex on
echo "TEST 8 - GET PAGE NOT FOUND AUTOINDEX ON EXPECTED 200"
check_status "GET" 200 "$SERVER1:$PORT1/pages/site1"

# Teste 9: GET de uma página não permitida
echo "TEST 9 - GET PAGE NOT ALLOWED EXPECTED 405"
check_status "GET" 405 "$SERVER2:$PORT2/"

# Teste 10: DELETE de um arquivo já deletado
echo "TEST 10 - DELETE INVALID FILE EXPECTED 404"
check_status "DELETE" 404 "$SERVER1:$PORT1/pages/site1/uploads/image_small.png"

# Teste 11: POST envio de arquivo (redirecionamento)
echo "TEST 11 - POST SEND BIG IMAGE EXPECTED 413 "
check_status "POST_FILE" 413 "$SERVER1:$PORT1/upload" "image_big.png"

echo "TEST 12 - POST CALC - EXPECTED 6.0"
check_status "POST" "6.0" "$SERVER1:$PORT1/sum/sum.html" "num1=2&num2=4"

echo "TEST 13 - POST CALC - NOT SEND NUMBERS"
check_status "POST" "<p>Por favor, preencha ambos os números.</p>" "$SERVER1:$PORT1/sum/sum.html" "num1=1&num4=4"

echo "TEST 14 - GET RANDOM MSG"
check_status "GET" 200 "$SERVER1:$PORT1/random"

echo "TEST 15 - GET REDIRECT"
check_status "GET" 301 "$SERVER1:$PORT1/mercadolivre"


rm -rf pages
rm -rf images
rm -rf cgi

# Encerre o servidor web
killall -9 webserv
# Mostrar o total de acertos e erros
echo "Quantidade de acertos: $acertos"
echo "Quantidade de erros: $erros"

echo "O teste foi concluído."

