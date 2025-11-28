#include <iostream>
#include <fstream>
#include <string>
#include "lexer.h"
#include "parser.h"

using namespace std;

string readFile(const string& filename) {
    ifstream in(filename);
    if (!in) throw runtime_error("No se pudo abrir el archivo");

    string content, line;
    while (getline(in, line)) {
        content += line + "\n";
    }
    return content;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Uso: mini0 <archivo.m0>\n";
        return 1;
    }

    try {
        string text = readFile(argv[1]);

        Lexer lexer(text);
        Token tok;
        bool hayErrorLex = false;
        Token errorTok;

        do {
            tok = lexer.nextToken();

            cout << "[Linea " << tok.line << "] Token: "
                 << tok.lexema << "  Tipo: "
                 << tokenToString(tok.type) << endl;

            if (tok.type == T_ERROR && !hayErrorLex) {
                hayErrorLex = true;
                errorTok = tok;
            }

        } while (tok.type != T_EOF);

        if (hayErrorLex) {
            cerr << "Error lexico en linea " << errorTok.line
                 << ": simbolo o literal invalido '" << errorTok.lexema
                 << "'\n";
            return 1;
        }

        Lexer lexer2(text);
        Parser parser(lexer2);
        parser.parse();

        return 0;

    } catch (const exception& e) {
        cerr << "ERROR: " << e.what() << endl;
        return 1;
    }
}
