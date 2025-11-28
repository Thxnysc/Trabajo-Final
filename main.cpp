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
        // Lee todo el archivo de entrada
        string text = readFile(argv[1]);

        // Creamos el lexer con el contenido del archivo
        Lexer lexer(text);

        // ===============================================
        // MODO MOSTRAR TOKENS
        // Aquí activamos el método que imprime
        // cada token, su tipo y los errores léxicos.
        // ===============================================
        lexer.scanAndPrint();
        return 0;


    } catch (...) {
        return 1;
    }
}

