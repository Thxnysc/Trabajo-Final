// ===============================================
// token.h
// Contiene los tipos de token y la estructura Token
// ===============================================

#ifndef TOKEN_H
#define TOKEN_H

#include <string>   // Usamos string para almacenar lexemas

using namespace std;

// Enumeracion con todos los tipos de token reconocidos por Mini-0
enum class TokenType {
    INT,        // Palabra clave 'int'
    IF,         // Palabra clave 'if'
    ELSE,       // Palabra clave 'else'
    ID,         // Identificador
    NUM,        // Numero entero
    LPAREN,     // '('
    RPAREN,     // ')'
    LBRACE,     // '{'
    RBRACE,     // '}'
    SEMI,       // ';'
    ASSIGN,     // '='
    LT,         // '<'
    GT,         // '>'
    EQEQ,       // '=='
    NEQ,        // '!='
    PLUS,       // '+'
    MINUS,      // '-'
    STAR,       // '*'
    SLASH,      // '/'
    END_OF_FILE,// Fin de archivo
    ERROR       // Token no reconocido
};

// Estructura que representa un token completo
struct Token {
    TokenType type;   // Tipo del token
    string lexeme;    // Texto original que aparecio en el archivo
    int line;         // Linea donde aparecio el token
};

#endif
