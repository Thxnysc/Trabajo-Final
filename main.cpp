// ===============================================
// main.cpp
// Punto de entrada del programa
// ===============================================

#include <iostream>
#include <fstream>
#include <string>
#include "lexer.h"
#include "parser.h"

using namespace std;

string readFile(const string& filename) {
    ifstream in(filename);
    if (!in) throw runtime_error("no se pudo abrir el archivo");

    string content, line;
    while (getline(in, line)) {
        content += line + "\n";
    }
    return content;
}

int main(int argc, char* argv[]) {

    if (argc < 2) {
        cerr << "Uso: parser <archivo.m0>" << endl;
        return 1;
    }

    try {
        string text = readFile(argv[1]);

        Lexer lexer(text);
        Parser parser(lexer);

        parser.program();

        cout << "Analisis sintactico exitoso. Programa valido." << endl;
        return 0;

    } catch (...) {
        return 1;
    }
}
