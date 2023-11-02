PORT=8080
URL="www.localhost:8080"

URL2="www.localhost:8080/99"

URLIMG="http://localhost:8081/upload"
URLDELTE="http://localhost:8081/delete.html"


#curl -v www.localhost:$PORT >> response_code.txt

###### GET ########

#### get de uma pagina valida
curl -s -o /dev/null -w "%{http_code}" $URL > response_code.txt
HTTP_STATUS=$(cat response_code.txt)
if [ "$HTTP_STATUS" = "200" ]; then
    echo "A solicitação foi bem-sucedida (código de status 200)."
else
    echo "A solicitação falhou (código de status $HTTP_STATUS)."
fi
rm response_code.txt


#### get de uma pagina invalida
curl -s -o /dev/null -w "%{http_code}" $URL2 > response_code.txt
HTTP_STATUS=$(cat response_code.txt)
if [ "$HTTP_STATUS" = "404" ]; then
    echo "A solicitação foi bem-sucedida (código de status $HTTP_STATUS)."
else
    echo "A solicitação falhou (código de status $HTTP_STATUS)."
fi
rm response_code.txt


### get de uma imagem invalida
curl -s -o /dev/null -w "%{http_code}" "http://localhost:8081/pages/site2/uploads/produto.png" > response_code.txt
HTTP_STATUS=$(cat response_code.txt)
if [ "$HTTP_STATUS" = "404" ]; then
    echo "A solicitação foi bem-sucedida (código de status $HTTP_STATUS)."
else
    echo "A solicitação falhou (código de status $HTTP_STATUS)."
fi
rm response_code.txt

### post envio de arquivo
IMAGE_FILE="produto.png"
curl -s -o /dev/null -X POST -w "%{http_code}" -F "imagem=@$IMAGE_FILE" $URLIMG > response_code.txt
HTTP_STATUS=$(cat response_code.txt)
if [ "$HTTP_STATUS" = "301" ]; then
    echo "A solicitação foi bem-sucedida (código de status $HTTP_STATUS)."
else
    echo "A solicitação falhou (código de status $HTTP_STATUS)."
fi

### get de uma imagem valida
curl -s -o /dev/null -w "%{http_code}" "http://localhost:8081/pages/site2/uploads/produto.png" > response_code.txt
HTTP_STATUS=$(cat response_code.txt)
if [ "$HTTP_STATUS" = "200" ]; then
    echo "A solicitação foi bem-sucedida (código de status $HTTP_STATUS)."
else
    echo "A solicitação falhou (código de status $HTTP_STATUS)."
fi
rm response_code.txt

### DELETA o arquivo enviado
curl -s -o /dev/null -X DELETE -w "%{http_code}" "http://localhost:8081/pages/site2/uploads/produto.png" > response_code.txt
HTTP_STATUS=$(cat response_code.txt)
if [ "$HTTP_STATUS" = "200" ]; then
    echo "A solicitação foi bem-sucedida (código de status $HTTP_STATUS)."
else
    echo "A solicitação falhou (código de status $HTTP_STATUS)."
fi

rm response_code.txt

### DELETA o arquivo enviado
curl -s -o /dev/null -X DELETE -w "%{http_code}" "http://localhost:8081/pages/site2/uploads/produto.png" > response_code.txt
HTTP_STATUS=$(cat response_code.txt)
if [ "$HTTP_STATUS" = "200" ]; then
    echo "A solicitação foi bem-sucedida (código de status $HTTP_STATUS)."
else
    echo "A solicitação falhou (código de status $HTTP_STATUS)."
fi

rm response_code.txt

















