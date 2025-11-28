#include "parser.h"
#include <iostream>
#include <cstdlib>

using namespace std;

// ===================================================================
// CONSTRUCTOR
// ===================================================================
Parser::Parser(Lexer& lex)
    : lexer(lex) 
{
    advance();
}

// ===================================================================
void Parser::advance() {
    current = lexer.nextToken();
}

// ===================================================================
bool Parser::check(TokenType t) {
    return current.type == t;
}

// ===================================================================
void Parser::expect(TokenType t) {
    if (!check(t)) {
        error("Se esperaba token '" + tokenToString(t) + "'");
    }
    advance();
}

// ===================================================================
void Parser::error(const string& msg) {
    cerr << "\n[Linea " << current.line << "] ERROR SINTACTICO\n";
    cerr << "  → " << msg << "\n";
    cerr << "  → Encontrado: '" << current.lexema << "'\n";
    exit(1);
}

// ===================================================================
void Parser::parse() {
    programa();

    if (!check(T_EOF)) {
        error("Se esperaba fin de archivo");
    }

    cout << "\nAnalisis sintactico exitoso.\n";
}

// ===================================================================
// ======================= GRAMATICA COMPLETA ========================
// ===================================================================

// programa → ListaNL decl ListaDecl
void Parser::programa() {
    ListaNL();
    decl();
    ListaDecl();
}

// ListaNL → NL ListaNL | ε
void Parser::ListaNL() {
    while (check(T_NL)) {
        advance();
    }
}

// ListaDecl → decl ListaDecl | ε
void Parser::ListaDecl() {
    while (check(T_FUN) || check(T_ID)) {
        decl();
    }
}

// decl → funcion | global
void Parser::decl() {
    if (check(T_FUN))
        funcion();
    else
        global_();
}

// global → declvar nl
void Parser::global_() {
    declvar();
    nl();
}

// funcion → 'fun' ID '(' params ')' TipoRet nl bloque 'end' nl
void Parser::funcion() {
    expect(T_FUN);
    expect(T_ID);
    expect(T_LPAREN);
    params();
    expect(T_RPAREN);
    TipoRet();
    nl();
    bloque();
    expect(T_END);
    nl();
}

// TipoRet → ':' tipo | ε
void Parser::TipoRet() {
    if (check(T_COLON)) {
        expect(T_COLON);
        tipo();
    }
}

// nl → NL ListaNL
void Parser::nl() {
    expect(T_NL);
    ListaNL();
}

// bloque → ListaDeclVar ListaComando
void Parser::bloque() {
    ListaDeclVar();
    ListaComando();
}

// ListaDeclVar → declvar nl ListaDeclVar | ε
void Parser::ListaDeclVar() {
    while (check(T_ID) && lexer.peekToken().type == T_COLON) {
        declvar();
        nl();
    }
}

// ListaComando → comando nl ListaComando | ε
void Parser::ListaComando() {
    while (
        check(T_IF) ||
        check(T_WHILE) ||
        check(T_RETURN) ||
        check(T_ID)
    ) {
        comando();
        nl();
    }
}

// params → ListaParam | ε
void Parser::params() {
    if (check(T_ID))
        ListaParam();
}

// ListaParam → parametro MasParam
void Parser::ListaParam() {
    parametro();
    MasParam();
}

// MasParam → ',' parametro MasParam | ε
void Parser::MasParam() {
    if (check(T_COMMA)) {
        expect(T_COMMA);
        parametro();
        MasParam();
    }
}

// parametro → ID ':' tipo
void Parser::parametro() {
    expect(T_ID);
    expect(T_COLON);
    tipo();
}

// declvar → ID ':' tipo
void Parser::declvar() {
    expect(T_ID);
    expect(T_COLON);
    tipo();
}

// tipo → tipobase | '[' ']' tipo
void Parser::tipo() {

    if (check(T_INT) || check(T_BOOL) || check(T_CHAR) || check(T_STRING)) {
        advance();
        return;
    }

    if (check(T_LBRACK)) {
        expect(T_LBRACK);
        expect(T_RBRACK);
        tipo();
        return;
    }

    error("Se esperaba un tipo o '[ ] tipo'");
}

// comando → cmdif | cmdwhile | cmdreturn | comandoID
void Parser::comando() {
    if (check(T_IF))         cmdif();
    else if (check(T_WHILE)) cmdwhile();
    else if (check(T_RETURN)) cmdreturn();
    else                     comandoID();
}

// comandoID → ID comandoID'
void Parser::comandoID() {
    expect(T_ID);
    comandoIDp();
}

// comandoID' → '(' listaexp ')' | VarSuffix '=' exp
void Parser::comandoIDp() {
    if (check(T_LPAREN)) {
        expect(T_LPAREN);
        listaexp();
        expect(T_RPAREN);
    } else {
        VarSuffix();
        expect(T_ASSIGN);
        exp();
    }
}

// cmdif → 'if' exp nl bloque ListaElseIf ElseOpt 'end'
void Parser::cmdif() {
    expect(T_IF);
    exp();
    nl();
    bloque();
    ListaElseIf();
    ElseOpt();
    expect(T_END);
}

// ListaElseIf → 'else' 'if' exp nl bloque ListaElseIf | ε
void Parser::ListaElseIf() {
    while (check(T_ELSE) && lexer.peekToken().type == T_IF) {
        expect(T_ELSE);
        expect(T_IF);
        exp();
        nl();
        bloque();
    }
}

// ElseOpt → 'else' nl bloque | ε
void Parser::ElseOpt() {
    if (check(T_ELSE)) {
        expect(T_ELSE);
        nl();
        bloque();
    }
}

// cmdwhile → 'while' exp nl bloque 'loop'
void Parser::cmdwhile() {
    expect(T_WHILE);
    exp();
    nl();
    bloque();
    expect(T_LOOP);
}

// cmdreturn → 'return' CmdReturn'
void Parser::cmdreturn() {
    expect(T_RETURN);
    CmdReturnp();
}

// CmdReturn' → exp | ε
void Parser::CmdReturnp() {
    if (
        check(T_ID) || check(T_NUM) || check(T_STR) ||
        check(T_TRUE) || check(T_FALSE) ||
        check(T_LPAREN) || check(T_NEW) ||
        check(T_MINUS) || check(T_NOT)
    ) {
        exp();
    }
}

// var → ID VarSuffix
void Parser::var() {
    expect(T_ID);
    VarSuffix();
}

// VarSuffix → '[' exp ']' VarSuffix | ε
void Parser::VarSuffix() {
    while (check(T_LBRACK)) {
        expect(T_LBRACK);
        exp();
        expect(T_RBRACK);
    }
}

// listaexp → ListaExpNoVacia | ε
void Parser::listaexp() {
    if (
        check(T_ID) || check(T_NUM) || check(T_STR) ||
        check(T_TRUE) || check(T_FALSE) ||
        check(T_LPAREN) || check(T_NEW) ||
        check(T_MINUS) || check(T_NOT)
    ) {
        ListaExpNoVacia();
    }
}

// ListaExpNoVacia → exp MasExp
void Parser::ListaExpNoVacia() {
    exp();
    MasExp();
}

// MasExp → ',' exp MasExp | ε
void Parser::MasExp() {
    if (check(T_COMMA)) {
        expect(T_COMMA);
        exp();
        MasExp();
    }
}

// exp → exp_or
void Parser::exp() {
    exp_or();
}

// exp_or → exp_and exp_or'
void Parser::exp_or() {
    exp_and();
    exp_orp();
}

// exp_or' → 'or' exp_and exp_or' | ε
void Parser::exp_orp() {
    while (check(T_OR)) {
        expect(T_OR);
        exp_and();
    }
}

// exp_and → exp_rel exp_and'
void Parser::exp_and() {
    exp_rel();
    exp_andp();
}

// exp_and' → 'and' exp_rel exp_and' | ε
void Parser::exp_andp() {
    while (check(T_AND)) {
        expect(T_AND);
        exp_rel();
    }
}

// exp_rel → exp_add exp_rel'
void Parser::exp_rel() {
    exp_add();
    exp_relp();
}

// exp_rel' → OpRel exp_add exp_rel' | ε
void Parser::exp_relp() {
    while (
        check(T_GT) || check(T_LT) ||
        check(T_GE) || check(T_LE) ||
        check(T_EQ) || check(T_NEQ)
    ) {
        advance();
        exp_add();
    }
}

// exp_add → exp_mul exp_add'
void Parser::exp_add() {
    exp_mul();
    exp_addp();
}

// exp_add' → OpAdd exp_mul exp_add' | ε
void Parser::exp_addp() {
    while (check(T_PLUS) || check(T_MINUS)) {
        advance();
        exp_mul();
    }
}

// exp_mul → exp_unary exp_mul'
void Parser::exp_mul() {
    exp_unary();
    exp_mulp();
}

// exp_mul' → OpMul exp_unary exp_mul' | ε
void Parser::exp_mulp() {
    while (check(T_MUL) || check(T_DIV)) {
        advance();
        exp_unary();
    }
}

// exp_unary → 'not' exp_unary | '-' exp_unary | exp_primary
void Parser::exp_unary() {
    if (check(T_NOT) || check(T_MINUS)) {
        advance();
        exp_unary();
    } else {
        exp_primary();
    }
}

// exp_primary → NUM | STR | true | false | new '[' exp ']' tipo | '(' exp ')' | ID RefResto
void Parser::exp_primary() {

    if (check(T_NUM) || check(T_STR) || check(T_TRUE) || check(T_FALSE)) {
        advance();
        return;
    }

    if (check(T_NEW)) {
        expect(T_NEW);
        expect(T_LBRACK);
        exp();
        expect(T_RBRACK);
        tipo();
        return;
    }

    if (check(T_LPAREN)) {
        expect(T_LPAREN);
        exp();
        expect(T_RPAREN);
        return;
    }

    if (check(T_ID)) {
        expect(T_ID);
        RefResto();
        return;
    }

    error("Se esperaba una expresion");
}

// RefResto → '(' listaexp ')' | VarSuffix
void Parser::RefResto() {
    if (check(T_LPAREN)) {
        expect(T_LPAREN);
        listaexp();
        expect(T_RPAREN);
    }
    else {
        VarSuffix();
    }
}

// ===================================================================
// TOKEN TO STRING (VERSION PROGRAMATICA, NO EN ESPAÑOL)
// ===================================================================
std::string tokenToString(TokenType t) {
    switch (t) {
        case T_FUN: return "FUN";
        case T_IF: return "IF";
        case T_ELSE: return "ELSE";
        case T_WHILE: return "WHILE";
        case T_END: return "END";
        case T_LOOP: return "LOOP";
        case T_RETURN: return "RETURN";
        case T_NEW: return "NEW";

        case T_TRUE: return "TRUE";
        case T_FALSE: return "FALSE";

        case T_INT: return "INT";
        case T_BOOL: return "BOOL";
        case T_CHAR: return "CHAR";
        case T_STRING: return "STRING";

        case T_OR: return "OR";
        case T_AND: return "AND";
        case T_NOT: return "NOT";

        case T_ID: return "ID";
        case T_NUM: return "NUM";
        case T_STR: return "STR";

        case T_GT: return "GT";
        case T_LT: return "LT";
        case T_GE: return "GE";
        case T_LE: return "LE";
        case T_EQ: return "EQ";
        case T_NEQ: return "NEQ";

        case T_PLUS: return "PLUS";
        case T_MINUS: return "MINUS";
        case T_MUL: return "MUL";
        case T_DIV: return "DIV";

        case T_ASSIGN: return "ASSIGN";

        case T_LBRACK: return "LBRACK";
        case T_RBRACK: return "RBRACK";
        case T_LPAREN: return "LPAREN";
        case T_RPAREN: return "RPAREN";
        case T_COMMA: return "COMMA";
        case T_COLON: return "COLON";

        case T_NL: return "NL";
        case T_EOF: return "EOF";
        case T_ERROR: return "ERROR";

        default: return "UNKNOWN";
    }
}
