PORT=8080
URL="www.localhost:8081"

INVALIDURL="www.localhost:8081/99"

URLIMG="http://localhost:8081/upload"
URLDELTE="http://localhost:8081/delete.html"


#curl -v www.localhost:$PORT >> response_code.txt

###### GET ########

#### get de uma pagina valida
echo "TEST 1 - GET VALID PAGE EXPECTED 200"
curl -s -o /dev/null -w "%{http_code}" $URL > response_code.txt
HTTP_STATUS=$(cat response_code.txt)
if [ "$HTTP_STATUS" = "200" ]; then
    echo "OK (código de status $HTTP_STATUS)."
else
    echo "FAIL (código de status $HTTP_STATUS)."
fi
rm response_code.txt
echo ""


#### get de uma pagina invalida
echo "TEST 2 - GET INVALID PAGE EXPECTED 404"
curl -s -o /dev/null -w "%{http_code}" $INVALIDURL > response_code.txt
HTTP_STATUS=$(cat response_code.txt)
if [ "$HTTP_STATUS" = "404" ]; then
    echo "OK (código de status $HTTP_STATUS)."
else
    echo "FAIL (código de status $HTTP_STATUS)."
fi
rm response_code.txt
echo ""


### get de uma imagem invalida
echo "TEST 3 - GET INVALID PAGE IMAGE EXPECTED 404"
curl -s -o /dev/null -w "%{http_code}" "http://localhost:8081/pages/site2/uploads/produto.png" > response_code.txt
HTTP_STATUS=$(cat response_code.txt)
if [ "$HTTP_STATUS" = "404" ]; then
    echo "OK (código de status $HTTP_STATUS)."
else
    echo "FAIL (código de status $HTTP_STATUS)."
fi
rm response_code.txt
echo ""

### post envio de arquivo
echo "TEST 4 - POST SEND IMAGE EXPECTED 301 - REDIRECT"
IMAGE_FILE="produto.png"
curl -s -o /dev/null -X POST -w "%{http_code}" -F "imagem=@$IMAGE_FILE" $URLIMG > response_code.txt
HTTP_STATUS=$(cat response_code.txt)
if [ "$HTTP_STATUS" = "301" ]; then
    echo "OK (código de status $HTTP_STATUS)."
else
    echo "FAIL (código de status $HTTP_STATUS)."
fi
rm response_code.txt
echo ""

### get de uma imagem valida

echo "TEST 5 - GET VALID PAGE IMAGE EXPECTED 200"
curl -s -o /dev/null -w "%{http_code}" "http://localhost:8081/pages/site2/uploads/produto.png" > response_code.txt
HTTP_STATUS=$(cat response_code.txt)
if [ "$HTTP_STATUS" = "200" ]; then
    echo "OK (código de status $HTTP_STATUS)."
else
    echo "FAIL (código de status $HTTP_STATUS)."
fi
rm response_code.txt
echo ""

### DELETA o arquivo enviado
echo "TEST 6 - DELETE FILE EXPECTED 200"
curl -s -o /dev/null -X DELETE -w "%{http_code}" "http://localhost:8081/pages/site2/uploads/produto.png" > response_code.txt
HTTP_STATUS=$(cat response_code.txt)
if [ "$HTTP_STATUS" = "200" ]; then
    echo "OK (código de status $HTTP_STATUS)."
else
    echo "FAIL (código de status $HTTP_STATUS)."
fi

rm response_code.txt
echo ""

### DELETA já deletado
echo "TEST 7 - DELETE INVALID FILE EXPECTED 404"
curl -s -o /dev/null -X DELETE -w "%{http_code}" "http://localhost:8080/pages/site1/uploads/produto.png" > response_code.txt
HTTP_STATUS=$(cat response_code.txt)
if [ "$HTTP_STATUS" = "404" ]; then
    echo "OK (código de status $HTTP_STATUS)."
else
    echo "FAIL (código de status $HTTP_STATUS)."
fi

rm response_code.txt
echo ""

### GET DE UMA PASTA NÃO PERMITIDA
echo "TEST 8 - GET PAGE NOT ALLOWED EXPECTED 405"
curl -s -o /dev/null -X DELETE -w "%{http_code}" "http://localhost:8080" > response_code.txt
HTTP_STATUS=$(cat response_code.txt)
if [ "$HTTP_STATUS" = "405" ]; then
    echo "OK (código de status $HTTP_STATUS)."
else
    echo "FAIL (código de status $HTTP_STATUS)."
fi

rm response_code.txt
echo ""


















