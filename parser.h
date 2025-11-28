#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include <string>

std::string tokenToString(TokenType t);

class Parser {
private:
    Lexer& lexer;
    Token current;

    void advance();
    bool check(TokenType t);
    void expect(TokenType t);
    void error(const std::string& msg);

    void programa();
    void ListaNL();
    void ListaDecl();
    void decl();
    void global_();
    void funcion();
    void TipoRet();
    void nl();
    void bloque();
    void ListaDeclVar();
    void ListaComando();

    void params();
    void ListaParam();
    void MasParam();
    void parametro();
    void declvar();
    void tipo();

    void comando();
    void comandoID();
    void comandoIDp();

    void cmdif();
    void ListaElseIf();
    void ElseOpt();

    void cmdwhile();
    void cmdreturn();
    void CmdReturnp();

    void var();
    void VarSuffix();

    void listaexp();
    void ListaExpNoVacia();
    void MasExp();

    void exp();
    void exp_or();
    void exp_orp();
    void exp_and();
    void exp_andp();
    void exp_rel();
    void exp_relp();
    void exp_add();
    void exp_addp();
    void exp_mul();
    void exp_mulp();
    void exp_unary();
    void exp_primary();
    void RefResto();

public:
    Parser(Lexer& lex);
    void parse();
};

#endif
