// ===============================================
// parser.h
// Cabecera del parser recursivo descendente
// ===============================================

#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "token.h"
#include <stdexcept>   // Para manejar errores

class Parser {
private:
    Lexer& lexer;     // Referencia al analizador lexico
    Token current;    // Token actual (lookahead)

    // Manejo de errores
    void syntaxError(const string& msg);
    void lexicalError(const Token& tok);

    // Funcion para consumir un token esperado
    void match(TokenType t);

    // Reglas de la gramatica Mini-0
    void decls();
    void decl();
    void stmts();
    void stmt();
    void stmt_else();
    void assign();
    void cond();
    void relop();
    void expr();
    void exprPrime();
    void term();
    void termPrime();
    void factor();

public:
    Parser(Lexer& lex); // Constructor
    void program();     // Punto inicial del analisis
};

#endif
