/*
Practica 5 - Analizador Lexico
Teoria de la Computacion
Garcia Ambrosio Aldo
Leaños Gutierrez Alan Rodrigo
Pérez Marquez David Andrawi
Red One
Grupo: 4CM5
Version   : 1.0
Descripcion: Lee un archivo fuente .c y detecta parejas de caracteres
             de agrupacion, mostrando cuantos hay de cada tipo.
*/

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

typedef struct {
    int parentesis;      /* () */
    int llaves;          /* {} */
    int corchetes;       /* [] */
    int triangulares;    /* <> */
} Contadores;


void detectarCaracteres(ifstream *archivo, Contadores *c) {
    char ch;
    c->parentesis = 0;
    c->llaves = 0;
    c->corchetes = 0;
    c->triangulares = 0;

    while ((archivo->get(ch))) {
        switch (ch) {
            case '(': c->parentesis++; break;
            case ')': c->parentesis++; break;
            case '{': c->llaves++; break;
            case '}': c->llaves++; break;
            case '[': c->corchetes++; break;
            case ']': c->corchetes++; break;
            case '<': c->triangulares++; break;
            case '>': c->triangulares++; break;
        }
    }
}
void mostrarResultados(const Contadores *c) {
    cout << "Resultados:\n";
    cout << "  Parentesis: " << c->parentesis << "\n";
    cout << "  Llaves: " << c->llaves << "\n";
    cout << "  Corchetes: " << c->corchetes << "\n";
    cout << "  Triangulares: " << c->triangulares << "\n";
    cout << "  Total: " << c->parentesis + c->llaves + c->corchetes + c->triangulares << "\n";
}

int main(int argc, char *argv[]) {
    Contadores contadores;
    if (argc < 2) {
        cerr << "Uso: " << argv[0] << " <archivo_fuente.c>\n";
        return EXIT_FAILURE;
    }
    ifstream archivo(argv[1]);
    if (!archivo.is_open()) {
        cerr << "Error al abrir el archivo: " << argv[1] << endl;
        return 1;
    }
    detectarCaracteres(&archivo, &contadores);
    mostrarResultados(&contadores);
    archivo.close();
    return 0;
}