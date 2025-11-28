// ===============================================
// lexer.cpp
// Implementacion del analizador lexico
// Con impresion de tokens linea por linea
// ===============================================

#include "lexer.h"
#include <cctype>
#include <iostream>

using namespace std;

// ---------------------------------------------------------------
// Funcion auxiliar para convertir un TokenType a string legible
// ---------------------------------------------------------------
string tokenTypeToString(TokenType t) {
    switch (t) {
        case TokenType::INT: return "INT";
        case TokenType::IF: return "IF";
        case TokenType::ELSE: return "ELSE";
        case TokenType::ID: return "ID";
        case TokenType::NUM: return "NUM";
        case TokenType::LPAREN: return "LPAREN";
        case TokenType::RPAREN: return "RPAREN";
        case TokenType::LBRACE: return "LBRACE";
        case TokenType::RBRACE: return "RBRACE";
        case TokenType::SEMI: return "SEMI";
        case TokenType::ASSIGN: return "ASSIGN";
        case TokenType::LT: return "LT";
        case TokenType::GT: return "GT";
        case TokenType::EQEQ: return "EQEQ";
        case TokenType::NEQ: return "NEQ";
        case TokenType::PLUS: return "PLUS";
        case TokenType::MINUS: return "MINUS";
        case TokenType::STAR: return "ASTERISK";   // <-- Aqui cambias el texto
        case TokenType::SLASH: return "SLASH";
        case TokenType::END_OF_FILE: return "EOF";
        case TokenType::ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

// ---------------------------------------------------------------
// Constructor del lexer
// ---------------------------------------------------------------
Lexer::Lexer(const string& text)
    : source(text), pos(0), line(1) {}

// ---------------------------------------------------------------
bool Lexer::isAtEnd() const {
    return pos >= source.size();
}

char Lexer::peek() const {
    if (isAtEnd()) return '\0';
    return source[pos];
}

char Lexer::peekNext() const {
    if (pos + 1 >= source.size()) return '\0';
    return source[pos + 1];
}

char Lexer::advance() {
    if (isAtEnd()) return '\0';
    char c = source[pos++];
    if (c == '\n') line++;
    return c;
}

// ---------------------------------------------------------------
void Lexer::skipWhitespaceAndComments() {
    while (!isAtEnd()) {
        char c = peek();

        if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
            advance();
        }
        else if (c == '/' && peekNext() == '/') {
            while (!isAtEnd() && peek() != '\n') advance();
        }
        else break;
    }
}

// ---------------------------------------------------------------
Token Lexer::identifierOrKeyword() {
    string lex;

    while (!isAtEnd() && (isalnum(peek()) || peek() == '_'))
        lex.push_back(advance());

    if (lex == "int") return {TokenType::INT, lex, line};
    if (lex == "if") return {TokenType::IF, lex, line};
    if (lex == "else") return {TokenType::ELSE, lex, line};

    return {TokenType::ID, lex, line};
}

// ---------------------------------------------------------------
Token Lexer::number() {
    string lex;
    while (!isAtEnd() && isdigit(peek()))
        lex.push_back(advance());
    return {TokenType::NUM, lex, line};
}

// ---------------------------------------------------------------
// nextToken: genera el siguiente token y LO IMPRIME
// ---------------------------------------------------------------
Token Lexer::nextToken() {

    skipWhitespaceAndComments();

    if (isAtEnd()) {
        Token t = {TokenType::END_OF_FILE, "", line};
        cout << "[Linea " << line << "] Token: <EOF>  Tipo: EOF" << endl;
        return t;
    }

    char c = advance();

    // Identificadores o palabras clave
    if (isalpha(c) || c == '_') {
        pos--;
        Token t = identifierOrKeyword();
        cout << "[Linea " << t.line << "] Token: " << t.lexeme
             << "  Tipo: " << tokenTypeToString(t.type) << endl;
        return t;
    }

    // Numeros
    if (isdigit(c)) {
        pos--;
        Token t = number();
        cout << "[Linea " << t.line << "] Token: " << t.lexeme
             << "  Tipo: " << tokenTypeToString(t.type) << endl;
        return t;
    }

    // Simbolos individuales y dobles
    switch (c) {

        case '(':
            cout << "[Linea " << line << "] Token: (  Tipo: "
                 << tokenTypeToString(TokenType::LPAREN) << endl;
            return {TokenType::LPAREN, "(", line};

        case ')':
            cout << "[Linea " << line << "] Token: )  Tipo: "
                 << tokenTypeToString(TokenType::RPAREN) << endl;
            return {TokenType::RPAREN, ")", line};

        case '{':
            cout << "[Linea " << line << "] Token: {  Tipo: "
                 << tokenTypeToString(TokenType::LBRACE) << endl;
            return {TokenType::LBRACE, "{", line};

        case '}':
            cout << "[Linea " << line << "] Token: }  Tipo: "
                 << tokenTypeToString(TokenType::RBRACE) << endl;
            return {TokenType::RBRACE, "}", line};

        case ';':
            cout << "[Linea " << line << "] Token: ;  Tipo: "
                 << tokenTypeToString(TokenType::SEMI) << endl;
            return {TokenType::SEMI, ";", line};

        case '+':
            cout << "[Linea " << line << "] Token: +  Tipo: "
                 << tokenTypeToString(TokenType::PLUS) << endl;
            return {TokenType::PLUS, "+", line};

        case '-':
            cout << "[Linea " << line << "] Token: -  Tipo: "
                 << tokenTypeToString(TokenType::MINUS) << endl;
            return {TokenType::MINUS, "-", line};

        case '*':
            cout << "[Linea " << line << "] Token: *  Tipo: "
                 << tokenTypeToString(TokenType::STAR) << endl;   
            return {TokenType::STAR, "*", line};                  

        case '/':
            cout << "[Linea " << line << "] Token: /  Tipo: "
                 << tokenTypeToString(TokenType::SLASH) << endl;
            return {TokenType::SLASH, "/", line};

        case '<':
            cout << "[Linea " << line << "] Token: <  Tipo: "
                 << tokenTypeToString(TokenType::LT) << endl;
            return {TokenType::LT, "<", line};

        case '>':
            cout << "[Linea " << line << "] Token: >  Tipo: "
                 << tokenTypeToString(TokenType::GT) << endl;
            return {TokenType::GT, ">", line};

        case '=':
            if (peek() == '=') {
                advance();
                cout << "[Linea " << line << "] Token: ==  Tipo: "
                     << tokenTypeToString(TokenType::EQEQ) << endl;
                return {TokenType::EQEQ, "==", line};
            }
            cout << "[Linea " << line << "] Token: =  Tipo: "
                 << tokenTypeToString(TokenType::ASSIGN) << endl;
            return {TokenType::ASSIGN, "=", line};

        case '!':
            if (peek() == '=') {
                advance();
                cout << "[Linea " << line << "] Token: !=  Tipo: "
                     << tokenTypeToString(TokenType::NEQ) << endl;
                return {TokenType::NEQ, "!=", line};
            }
            cout << "[Linea " << line << "] Token: !  Tipo: "
                 << tokenTypeToString(TokenType::ERROR) << endl;
            return {TokenType::ERROR, "!", line};
    }

    // Error léxico
    string lexeme(1, c);
    cout << "[Linea " << line << "] Token: " << lexeme
         << "  Tipo: ERROR" << endl;
    return {TokenType::ERROR, lexeme, line};
}
