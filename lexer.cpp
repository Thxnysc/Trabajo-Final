// ===============================================
// lexer.cpp
// Implementacion del analizador lexico
// ===============================================

#include "lexer.h"
#include <cctype>   // Para funciones isdigit, isalpha, etc

// Constructor: inicializa el texto, posicion y linea
Lexer::Lexer(const string& text)
    : source(text), pos(0), line(1) {}

// Indica si llegamos al final del archivo
bool Lexer::isAtEnd() const {
    return pos >= source.size();
}

// Devuelve el caracter actual sin avanzar
char Lexer::peek() const {
    if (isAtEnd()) return '\0';
    return source[pos];
}

// Devuelve el siguiente caracter sin avanzar
char Lexer::peekNext() const {
    if (pos + 1 >= source.size()) return '\0';
    return source[pos + 1];
}

// Avanza un caracter y lo devuelve
char Lexer::advance() {
    if (isAtEnd()) return '\0';
    char c = source[pos++];
    if (c == '\n') line++; // Si es salto de linea, actualiza el contador
    return c;
}

// Salta espacios en blanco y comentarios
void Lexer::skipWhitespaceAndComments() {
    while (!isAtEnd()) {
        char c = peek();

        if (c == ' ' || c == '\t' || c == '\r' || c == '\n')
            advance();  // Ignora espacios
        else if (c == '/' && peekNext() == '/')
            while (!isAtEnd() && peek() != '\n') advance(); // Comentario de linea
        else
            break; // Si ya no es espacio ni comentario, salir
    }
}

// Procesa identificadores o palabras clave
Token Lexer::identifierOrKeyword() {
    string lex;

    while (!isAtEnd() && (isalnum(peek()) || peek() == '_'))
        lex.push_back(advance());  // Agrega cada caracter al lexema

    // Verifica palabras clave
    if (lex == "int") return {TokenType::INT, lex, line};
    if (lex == "if") return {TokenType::IF, lex, line};
    if (lex == "else") return {TokenType::ELSE, lex, line};

    return {TokenType::ID, lex, line}; // Si no es keyword, es ID
}

// Procesa numeros
Token Lexer::number() {
    string lex;
    while (!isAtEnd() && isdigit(peek()))
        lex.push_back(advance());
    return {TokenType::NUM, lex, line};
}

// Devuelve el siguiente token del archivo
Token Lexer::nextToken() {
    skipWhitespaceAndComments(); // Primero ignora basura

    if (isAtEnd())
        return {TokenType::END_OF_FILE, "", line};

    char c = advance();

    // Identificadores
    if (isalpha(c) || c == '_') { pos--; return identifierOrKeyword(); }

    // Numeros
    if (isdigit(c)) { pos--; return number(); }

    // Simbolos individuales
    switch (c) {
        case '(': return {TokenType::LPAREN, "(", line};
        case ')': return {TokenType::RPAREN, ")", line};
        case '{': return {TokenType::LBRACE, "{", line};
        case '}': return {TokenType::RBRACE, "}", line};
        case ';': return {TokenType::SEMI, ";", line};
        case '+': return {TokenType::PLUS, "+", line};
        case '-': return {TokenType::MINUS, "-", line};
        case '*': return {TokenType::STAR, "*", line};
        case '/': return {TokenType::SLASH, "/", line};
        case '<': return {TokenType::LT, "<", line};
        case '>': return {TokenType::GT, ">", line};

        case '=':
            if (peek() == '=') { advance(); return {TokenType::EQEQ, "==", line}; }
            return {TokenType::ASSIGN, "=", line};

        case '!':
            if (peek() == '=') { advance(); return {TokenType::NEQ, "!=", line}; }
            return {TokenType::ERROR, "!", line};
    }

    // Caracter desconocido
    string lex(1, c);
    return {TokenType::ERROR, lex, line};
}
