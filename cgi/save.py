import cgi
import os

upload_dir = '/uploads'

form = cgi.FieldStorage()

if "file" in form:
    fileitem = form["file"]
    if fileitem.filename:
        filepath = os.path.join(upload_dir, os.path.basename(fileitem.filename))
        with open(filepath, 'wb') as f:
            f.write(fileitem.file.read())
        print("Content-type: text/html\n")
        print("<html>")
        print("<head><title>Upload Concluído</title></head>")
        print("<body>")
        print("<h1>Upload Concluído</h1>")
        print(f"<p>O arquivo '{fileitem.filename}' foi enviado e salvo em '{filepath}'</p>")
        print("</body>")
        print("</html>")
    else:
        print("Content-type: text/html\n")
        print("<html>")
        print("<head><title>Erro no Upload</title></head>")
        print("<body>")
        print("<h1>Erro no Upload</h1>")
        print("<p>Nenhum arquivo foi selecionado.</p>")
        print("</body>")
        print("</html>")
else:
    print("Content-type: text/html\n")
    print("<html>")
    print("<head><title>Formulário de Upload</title></head>")
    print("<body>")
    print("<h1>Formulário de Upload</h1>")
    print('<form enctype="multipart/form-data" method="post">')
    print('<label for="file">Selecione um arquivo:</label>')
    print('<input type="file" id="file" name="file"><br><br>')
    print('<input type="submit" value="Enviar">')
    print('</form>')
    print("</body>")
    print("</html>")