#include "lexer.h"
#include <iostream>
#include <cctype>

using namespace std;

Lexer::Lexer(const string& src)
    : text(src), pos(0), line(1) {}

char Lexer::peek() {
    return pos < (int)text.size() ? text[pos] : '\0';
}

char Lexer::get() {
    return pos < (int)text.size() ? text[pos++] : '\0';
}

bool Lexer::eof() {
    return pos >= (int)text.size();
}

Token Lexer::nextToken() {
    Token tok;
    tok.lexema = "";
    tok.line = line;

    while (!eof() && (peek() == ' ' || peek() == '\t' || peek() == '\r'))
        get();

    if (!eof() && peek() == '\n') {
        get();
        tok.type = T_NL;
        tok.lexema = "NL";
        line++;
        return tok;
    }

    if (eof()) {
        tok.type = T_EOF;
        return tok;
    }

    char c = peek();

    if (c == '/' && pos + 1 < (int)text.size() && text[pos+1] == '/') {
        get();
        get();
        while (!eof() && peek() != '\n')
            get();
        return nextToken();
    }

    if (isalpha(c) || c == '_') {
        while (!eof() && (isalnum(peek()) || peek() == '_'))
            tok.lexema += get();

        if      (tok.lexema == "fun") tok.type = T_FUN;
        else if (tok.lexema == "if") tok.type = T_IF;
        else if (tok.lexema == "else") tok.type = T_ELSE;
        else if (tok.lexema == "while") tok.type = T_WHILE;
        else if (tok.lexema == "end") tok.type = T_END;
        else if (tok.lexema == "loop") tok.type = T_LOOP;
        else if (tok.lexema == "return") tok.type = T_RETURN;
        else if (tok.lexema == "new") tok.type = T_NEW;
        else if (tok.lexema == "true") tok.type = T_TRUE;
        else if (tok.lexema == "false") tok.type = T_FALSE;
        else if (tok.lexema == "int") tok.type = T_INT;
        else if (tok.lexema == "bool") tok.type = T_BOOL;
        else if (tok.lexema == "char") tok.type = T_CHAR;
        else if (tok.lexema == "string") tok.type = T_STRING;
        else if (tok.lexema == "or") tok.type = T_OR;
        else if (tok.lexema == "and") tok.type = T_AND;
        else if (tok.lexema == "not") tok.type = T_NOT;
        else tok.type = T_ID;

        return tok;
    }

    if (isdigit(c)) {
        while (!eof() && isdigit(peek()))
            tok.lexema += get();

        if (!eof() && isalpha(peek())) {
            while (!eof() && isalnum(peek()))
                tok.lexema += get();
            tok.type = T_ERROR;
            return tok;
        }

        tok.type = T_NUM;
        return tok;
    }

    if (c == '"') {
        get();
        bool closed = false;

        while (!eof()) {
            if (peek() == '"') {
                get();
                closed = true;
                break;
            }
            if (peek() == '\n') break;
            tok.lexema += get();
        }

        if (!closed) {
            tok.type = T_ERROR;
            return tok;
        }

        tok.type = T_STR;
        return tok;
    }

    if (!eof()) {
        char n = (pos + 1 < (int)text.size()) ? text[pos+1] : '\0';

        if (c == '>' && n == '=') { get(); get(); tok.type = T_GE; tok.lexema = ">="; return tok; }
        if (c == '<' && n == '=') { get(); get(); tok.type = T_LE; tok.lexema = "<="; return tok; }
        if (c == '=' && n == '=') { get(); get(); tok.type = T_EQ; tok.lexema = "=="; return tok; }
        if (c == '<' && n == '>') { get(); get(); tok.type = T_NEQ; tok.lexema = "<>"; return tok; }
    }

    get();

    switch (c) {
        case '>': tok.type = T_GT; tok.lexema = ">"; return tok;
        case '<': tok.type = T_LT; tok.lexema = "<"; return tok;
        case '+': tok.type = T_PLUS; tok.lexema = "+"; return tok;
        case '-': tok.type = T_MINUS; tok.lexema = "-"; return tok;
        case '*': tok.type = T_MUL; tok.lexema = "*"; return tok;
        case '/': tok.type = T_DIV; tok.lexema = "/"; return tok;
        case '=': tok.type = T_ASSIGN; tok.lexema = "="; return tok;
        case '[': tok.type = T_LBRACK; tok.lexema = "["; return tok;
        case ']': tok.type = T_RBRACK; tok.lexema = "]"; return tok;
        case '(' : tok.type = T_LPAREN; tok.lexema = "("; return tok;
        case ')' : tok.type = T_RPAREN; tok.lexema = ")"; return tok;
        case ',' : tok.type = T_COMMA; tok.lexema = ","; return tok;
        case ':' : tok.type = T_COLON; tok.lexema = ":"; return tok;
    }

    tok.type = T_ERROR;
    tok.lexema = string(1, c);
    return tok;
}

Token Lexer::peekToken() {
    int oldPos = pos;
    int oldLine = line;
    Token t = nextToken();
    pos = oldPos;
    line = oldLine;
    return t;
}

void Lexer::scanAndPrint() {
    Token tok;
    do {
        tok = nextToken();
        cout << tok.lexema << " (" << tok.type << ")\n";
    } while (tok.type != T_EOF);
}
