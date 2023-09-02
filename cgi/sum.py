#!/usr/bin/env python3
import cgi

def sum_numbers(num1, num2):
    try:
        result = float(num1) + float(num2)
        return result
    except ValueError:
        return "Erro: Entradas inválidas"

form = cgi.FieldStorage()
if "num1" in form and "num2" in form:
    num1 = form["num1"].value
    num2 = form["num2"].value
    result = sum_numbers(num1, num2)
    print(f"Resultado: {result}")
else:
    print("<p>Por favor, preencha ambos os números.</p>")
