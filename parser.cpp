// ===============================================
// parser.cpp
// Implementacion del parser recursivo descendente
// Cada linea esta comentada para explicar su funcion
// ===============================================

#include "parser.h"     // Incluimos la cabecera del parser
#include <iostream>      // Para imprimir errores o mensajes

using namespace std;

// ---------------------------------------------------------------
// Constructor del parser
// ---------------------------------------------------------------
Parser::Parser(Lexer& lex)      // El constructor recibe una referencia al lexer
    : lexer(lex)                // Guardamos la referencia en el atributo interno
{
    current = lexer.nextToken(); // Leemos el primer token inmediatamente
}

// ===============================================================
// Recorre TODO el código fuente y va imprimiendo:
// ===============================================================
void Lexer::scanAndPrint() {

    Token tok;  // Variable que almacenará el token actual

    do {
        // Pedimos el siguiente token del analizador léxico
        tok = nextToken();

        // Imprimimos la línea donde fue encontrado
        cout << "Linea " << tok.line << " -> ";

        // Mostramos el texto tal como aparece en el archivo
        cout << "Lexema: '" << tok.lexeme << "'  |  ";

        // Dependiendo del tipo de token, escribimos su nombre
        switch (tok.type) {

            case TokenType::INT:
                cout << "Token: INT"; 
                break;

            case TokenType::IF:
                cout << "Token: IF"; 
                break;

            case TokenType::ELSE:
                cout << "Token: ELSE"; 
                break;

            case TokenType::ID:
                cout << "Token: ID"; 
                break;

            case TokenType::NUM:
                cout << "Token: NUM"; 
                break;

            case TokenType::LPAREN:
                cout << "Token: LPAREN"; 
                break;

            case TokenType::RPAREN:
                cout << "Token: RPAREN"; 
                break;

            case TokenType::LBRACE:
                cout << "Token: LBRACE"; 
                break;

            case TokenType::RBRACE:
                cout << "Token: RBRACE"; 
                break;

            case TokenType::SEMI:
                cout << "Token: SEMI"; 
                break;

            case TokenType::ASSIGN:
                cout << "Token: ASSIGN"; 
                break;

            case TokenType::PLUS:
                cout << "Token: PLUS"; 
                break;

            case TokenType::MINUS:
                cout << "Token: MINUS"; 
                break;

            case TokenType::STAR:
                cout << "Token: STAR"; 
                break;

            case TokenType::SLASH:
                cout << "Token: SLASH"; 
                break;

            case TokenType::LT:
                cout << "Token: LT"; 
                break;

            case TokenType::GT:
                cout << "Token: GT"; 
                break;

            case TokenType::EQEQ:
                cout << "Token: EQEQ"; 
                break;

            case TokenType::NEQ:
                cout << "Token: NEQ"; 
                break;

            case TokenType::END_OF_FILE:
                cout << "Token: EOF"; 
                break;

            // Cuando el token es ERROR, aquí lo mostramos claramente
            case TokenType::ERROR:
                cout << "ERROR: simbolo no reconocido"; 
                break;
        }

        cout << endl; // Salto de línea después de cada token impreso

    } while (tok.type != TokenType::END_OF_FILE); // Termina al llegar a EOF
}

// ---------------------------------------------------------------
// Manejo de errores sintacticos
// ---------------------------------------------------------------
void Parser::syntaxError(const string& msg) {
    cerr << "Error sintactico en linea "
         << current.line                  // Imprime en que linea ocurrio el error
         << ": " << msg                   // Imprime el mensaje especifico
         << " (token: '" << current.lexeme << "')"  // Indica el lexema problematico
         << endl;
    throw runtime_error("syntax");         // Lanza una excepcion para detener el analisis
}

// ---------------------------------------------------------------
// Manejo de errores lexicos
// ---------------------------------------------------------------
void Parser::lexicalError(const Token& tok) {
    cerr << "Error lexico en linea "
         << tok.line                       // Linea donde esta el error lexico
         << ": simbolo '" << tok.lexeme    // Caracter problematico
         << "' no reconocido" << endl;
    throw runtime_error("lexical");        // Detiene el parser
}

// ---------------------------------------------------------------
// match()
// Verifica que el token actual sea el esperado, si lo es avanza,
// si no, lanza error
// ---------------------------------------------------------------
void Parser::match(TokenType t) {

    if (current.type == TokenType::ERROR)   // Si el token actual es un token de error
        lexicalError(current);              // Lanzamos error lexico

    if (current.type == t)                  // Si coincide con lo esperado
        current = lexer.nextToken();        // Avanzamos al siguiente token
    else
        syntaxError("se esperaba otro token"); // Error si no coincide
}

// ===============================================================
// AQUI EMPIEZAN LAS REGLAS DE LA GRAMATICA MINI-0
// ===============================================================

// ---------------------------------------------------------------
// program -> decls stmts
// ---------------------------------------------------------------
void Parser::program() {
    decls();                                 // Analiza las declaraciones
    stmts();                                 // Analiza las sentencias

    if (current.type != TokenType::END_OF_FILE)  // Si quedan tokens despues del analisis
        syntaxError("se esperaba fin de archivo"); // El programa es incorrecto
}

// ---------------------------------------------------------------
// decls -> decl decls | epsilon
// ---------------------------------------------------------------
void Parser::decls() {

    if (current.type == TokenType::INT) {    // Si inicia con 'int', hay una declaracion
        decl();                              // Procesa la declaracion
        decls();                             // LLamado recursivo para mas declaraciones
    }
    // Si no empieza con 'int', significa epsilon
}

// ---------------------------------------------------------------
// decl -> int ID ;
// ---------------------------------------------------------------
void Parser::decl() {

    match(TokenType::INT);                    // Consume la palabra clave int
    match(TokenType::ID);                     // Consume el identificador
    match(TokenType::SEMI);                   // Consume el punto y coma
}

// ---------------------------------------------------------------
// stmts -> stmt stmts | epsilon
// ---------------------------------------------------------------
void Parser::stmts() {

    if (current.type == TokenType::ID ||      // Si la sentencia inicia con ID
        current.type == TokenType::IF ||      // o con if
        current.type == TokenType::LBRACE)    // o con '{'
    {
        stmt();                               // Procesamos una sentencia
        stmts();                              // LLamado recursivo para mas sentencias
    }
    // Si no coincide, es epsilon
}

// ---------------------------------------------------------------
// stmt -> assign ';'
//       | if '(' cond ')' stmt stmt_else
//       | '{' stmts '}'
// ---------------------------------------------------------------
void Parser::stmt() {

    if (current.type == TokenType::ID) {       // Caso: sentencia de asignacion
        assign();                              // Procesa la asignacion
        match(TokenType::SEMI);                // Consume ';'
    }

    else if (current.type == TokenType::IF) {  // Caso: if
        match(TokenType::IF);                  // Consume 'if'
        match(TokenType::LPAREN);              // Consume '('
        cond();                                // Procesa la condicion
        match(TokenType::RPAREN);              // Consume ')'
        stmt();                                // Procesa la sentencia del if
        stmt_else();                           // Procesa opcionalmente el else
    }

    else if (current.type == TokenType::LBRACE) {  // Caso: bloque '{ ... }'
        match(TokenType::LBRACE);                  // Consume '{'
        stmts();                                   // Procesa las sentencias internas
        match(TokenType::RBRACE);                  // Consume '}'
    }

    else {
        syntaxError("sentencia no valida");   // Ningun caso coincide -> error
    }
}

// ---------------------------------------------------------------
// stmt_else -> else stmt | epsilon
// ---------------------------------------------------------------
void Parser::stmt_else() {

    if (current.type == TokenType::ELSE) {     // Si aparece un else
        match(TokenType::ELSE);                // Consume 'else'
        stmt();                                // Procesa la sentencia luego de else
    }
    // Si no aparece 'else', esta regla produce epsilon
}

// ---------------------------------------------------------------
// assign -> ID '=' expr
// ---------------------------------------------------------------
void Parser::assign() {

    match(TokenType::ID);                      // Consume el identificador
    match(TokenType::ASSIGN);                  // Consume '='
    expr();                                    // Procesa la expresion de la derecha
}

// ---------------------------------------------------------------
// cond -> expr relop expr
// ---------------------------------------------------------------
void Parser::cond() {
    expr();                                    // Primera expresion
    relop();                                   // Operador relacional
    expr();                                    // Segunda expresion
}

// ---------------------------------------------------------------
// relop -> '<' | '>' | '==' | '!='
// ---------------------------------------------------------------
void Parser::relop() {

    if (current.type == TokenType::LT)         // Caso '<'
        match(TokenType::LT);

    else if (current.type == TokenType::GT)    // Caso '>'
        match(TokenType::GT);

    else if (current.type == TokenType::EQEQ)  // Caso '=='
        match(TokenType::EQEQ);

    else if (current.type == TokenType::NEQ)   // Caso '!='
        match(TokenType::NEQ);

    else
        syntaxError("operador relacional esperado"); // Si no hay ninguno -> error
}

// ---------------------------------------------------------------
// expr -> term expr'
// ---------------------------------------------------------------
void Parser::expr() {
    term();                                    // Procesa un termino
    exprPrime();                               // Procesa sumas/restas opcionales
}

// ---------------------------------------------------------------
// expr' -> + term expr' | - term expr' | epsilon
// ---------------------------------------------------------------
void Parser::exprPrime() {

    if (current.type == TokenType::PLUS) {     // Caso '+'
        match(TokenType::PLUS);                // Consume '+'
        term();                                // Procesa siguiente termino
        exprPrime();                           // Sigue analizando expr'
    }

    else if (current.type == TokenType::MINUS) { // Caso '-'
        match(TokenType::MINUS);               // Consume '-'
        term();                                // Procesa siguiente termino
        exprPrime();                           // Sigue con expr'
    }

    // Si no es + ni -, es epsilon
}

// ---------------------------------------------------------------
// term -> factor term'
// ---------------------------------------------------------------
void Parser::term() {
    factor();                                  // Procesa un factor
    termPrime();                               // Procesa multiplicaciones/divisiones opcionales
}

// ---------------------------------------------------------------
// term' -> * factor term' | / factor term' | epsilon
// ---------------------------------------------------------------
void Parser::termPrime() {

    if (current.type == TokenType::STAR) {     // Caso '*'
        match(TokenType::STAR);                // Consume '*'
        factor();                              // Procesa el factor
        termPrime();                           // Llamado recursivo
    }

    else if (current.type == TokenType::SLASH) { // Caso '/'
        match(TokenType::SLASH);               // Consume '/'
        factor();                              // Procesa el factor
        termPrime();                           // Llamado recursivo
    }

    // Si no coincide, epsilon
}

// ---------------------------------------------------------------
// factor -> ID | NUM | '(' expr ')'
// ---------------------------------------------------------------
void Parser::factor() {

    if (current.type == TokenType::ID)          // Caso ID
        match(TokenType::ID);

    else if (current.type == TokenType::NUM)    // Caso NUM
        match(TokenType::NUM);

    else if (current.type == TokenType::LPAREN) {// Caso '(' expr ')'
        match(TokenType::LPAREN);               // Consume '('
        expr();                                 // Procesa expresion interna
        match(TokenType::RPAREN);               // Consume ')'
    }

    else
        syntaxError("factor no valido");        // Si nada coincide, error
}
