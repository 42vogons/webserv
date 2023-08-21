#!/usr/bin/env python
# -*- coding: UTF-8 -*-

import cgi
import os

# Configurar o cabeçalho da resposta
print("Content-Type: text/html\n")

# Ler os dados do formulário
form = cgi.FieldStorage()

if "file" in form:
    fileitem = form["file"]

    if fileitem.file:
        # Caminho onde o arquivo será salvo
        filepath = os.path.join("upload", fileitem.filename)

        # Salvar o arquivo
        with open(filepath, "wb") as output_file:
            while True:
                chunk = fileitem.file.read(8192)
                if not chunk:
                    break
                output_file.write(chunk)

        print("<h2>Arquivo '{}' foi enviado e salvo com sucesso!</h2>".format(fileitem.filename))
    else:
        print("<h2>Erro: Nenhum arquivo foi enviado.</h2>")
else:
    print("<h2>Erro: Nenhum arquivo foi encontrado no formulário.</h2>")