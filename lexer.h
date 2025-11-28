// ===============================================
// lexer.h
// Cabecera del analizador lexico
// ===============================================

#ifndef LEXER_H
#define LEXER_H

#include <string>      // Para manejar cadenas
#include "token.h"     // Incluye la definicion de Token y TokenType

class Lexer {
private:
    string source;     // Contenido completo del archivo fuente
    size_t pos;        // Posicion actual dentro del texto
    int line;          // Numero de linea donde estamos

    bool isAtEnd() const;      // Verifica si llegamos al final del archivo
    char peek() const;         // Mira el caracter actual sin avanzar
    char peekNext() const;     // Mira el siguiente caracter
    char advance();            // Avanza un caracter y lo devuelve
    void skipWhitespaceAndComments();  // Ignora espacios y comentarios
    Token identifierOrKeyword();       // Procesa identificadores o palabras clave
    Token number();                    // Procesa numeros

public:
    Lexer(const string& text);  // Constructor que recibe el codigo fuente completo
    Token nextToken();          // Devuelve el siguiente token
    void scanAndPrint();        // Imprime linea por linea
};

#endif
