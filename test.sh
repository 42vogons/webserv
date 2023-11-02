PORT=8080
URL="www.localhost:8080"

URL2="www.localhost:8080/99"

URLIMG="http://localhost:8081/upload"


curl -s -o /dev/null -w "%{http_code}" "http://localhost:8081/pages/site2/uploads/rg_frente.jpeg" > response_code.txt
HTTP_STATUS=$(cat response_code.txt)
if [ "$HTTP_STATUS" = "404" ]; then
    echo "A solicitação foi bem-sucedida (código de status 404)."
else
    echo "A solicitação falhou (código de status $HTTP_STATUS)."
fi
rm response_code.txt



#curl -v www.localhost:$PORT >> response_code.txt

###### GET ########

IMAGE_FILE="1234.png"

#curl -X POST -F @1234.png http://localhost:8081/upload.html

curl -X POST -F "imagem=@$IMAGE_FILE" $URLIMG

#curl -v $URL

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
    echo "A solicitação foi bem-sucedida (código de status 404)."
else
    echo "A solicitação falhou (código de status $HTTP_STATUS)."
fi
rm response_code.txt


### get de uma imagem invalida
curl -s -o /dev/null -w "%{http_code}" "http://localhost:8081/pages/site2/uploads/rg_frente.jpeg" > response_code.txt
HTTP_STATUS=$(cat response_code.txt)
if [ "$HTTP_STATUS" = "404" ]; then
    echo "A solicitação foi bem-sucedida (código de status 404)."
else
    echo "A solicitação falhou (código de status $HTTP_STATUS)."
fi
rm response_code.txt













