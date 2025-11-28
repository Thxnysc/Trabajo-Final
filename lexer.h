#ifndef LEXER_H
#define LEXER_H

#include <string>

enum TokenType {
    // Palabras clave
    T_FUN, T_IF, T_ELSE, T_WHILE, T_END, T_LOOP, T_RETURN,
    T_NEW, T_TRUE, T_FALSE,

    // Tipos base
    T_INT, T_BOOL, T_CHAR, T_STRING,

    // Operadores lógicos
    T_OR, T_AND, T_NOT,

    // Identificadores y literales
    T_ID,
    T_NUM,
    T_STR,

    // Operadores relacionales
    T_GT, T_LT, T_GE, T_LE, T_EQ, T_NEQ,

    // Operadores aritméticos
    T_PLUS, T_MINUS, T_MUL, T_DIV,

    // Asignación
    T_ASSIGN,

    // Delimitadores
    T_LBRACK, T_RBRACK,
    T_LPAREN, T_RPAREN,
    T_COMMA, T_COLON,

    // Nueva línea
    T_NL,

    // Fin archivo
    T_EOF,

    // Error
    T_ERROR
};

struct Token {
    TokenType type;
    std::string lexema;
    int line;
};

class Lexer {
private:
    std::string text;
    int pos;
    int line;

    char peek();
    char get();
    bool eof();

public:
    Lexer(const std::string& src);

    Token nextToken();
    Token peekToken();
    void scanAndPrint();
};

#endif
