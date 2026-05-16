/*
Practica 7 - Analizador Lexico de Codigo Fuente
Descripcion: Detecta y contabiliza simbolos lexicos en un archivo fuente .C
           muestra cantidad y porcentaje de aparicion de cada token.
Version: 3.0
Mayo 2026
Autores: Garcia Ambrosio Aldo
         Leanos Gutierrez Alan Rodrigo
         Perez Marquez David Andrawi
Equipo: Red One
 */

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

typedef struct {
    int caracter;      // '  
    int cadena;        // " 
    int includes;      // < > 
    int corrBits;      // << >> 
    int relComp;       // <= >= 
    int igualdad;      // == !=
    int comentario;    // //  
    int comentMulti;   // /* */ 
    int ternario;      // ?  
    int puntero;       // ->  
    int accesoDir;     // .   
    int sepPara;       // ,   
    int finInstr;      // ;   
    int etqcase;       // ::  
} Contadores;

/* Recorre el archivo caracter por caracter detectando tokens */
void detectarCaracteres(ifstream *archivo, Contadores *c) {

    char ch, next;

    c->caracter    = 0;
    c->cadena      = 0;
    c->includes    = 0;
    c->corrBits    = 0;
    c->relComp     = 0;
    c->igualdad    = 0;
    c->comentario  = 0;
    c->comentMulti = 0;
    c->ternario    = 0;
    c->puntero     = 0;
    c->accesoDir   = 0;
    c->sepPara     = 0;
    c->finInstr    = 0;
    c->etqcase     = 0;

    while (archivo->get(ch)) {

        next = (archivo->peek() != EOF) ? (char)archivo->peek() : '\0';

        /* Pares de dos caracteres (se consume el segundo) */
        if      (ch == '<' && next == '<') { c->corrBits++;    archivo->get(ch); }
        else if (ch == '>' && next == '>') { c->corrBits++;    archivo->get(ch); }
        else if (ch == '<' && next == '=') { c->relComp++;     archivo->get(ch); }
        else if (ch == '>' && next == '=') { c->relComp++;     archivo->get(ch); }
        else if (ch == '=' && next == '=') { c->igualdad++;    archivo->get(ch); }
        else if (ch == '!' && next == '=') { c->igualdad++;    archivo->get(ch); }
        else if (ch == '/' && next == '/') { c->comentario++;  archivo->get(ch); }
        else if (ch == '/' && next == '*') { c->comentMulti++; archivo->get(ch); }
        else if (ch == '-' && next == '>') { c->puntero++;     archivo->get(ch); }
        else if (ch == ':' && next == ':') { c->etqcase++;     archivo->get(ch); }

        /* Caracteres simples */
        else if (ch == '<' || ch == '>') { c->includes++;  }
        else if (ch == '\'')             { c->caracter++;  }
        else if (ch == '"')              { c->cadena++;    }
        else if (ch == '?')              { c->ternario++;  }
        else if (ch == '.')              { c->accesoDir++; }
        else if (ch == ',')              { c->sepPara++;   }
        else if (ch == ';')              { c->finInstr++;  }
    }
}

/* Imprime una fila de la tabla de resultados */
void imprimirFila(const string &nombre, int cantidad, int total) {
    double pct = (total > 0) ? (cantidad * 100.0 / total) : 0.0;
    cout << left  << setw(36) << nombre
         << right << setw(8)  << cantidad
         << setw(12) << fixed << setprecision(2) << pct << "%\n";
}

int main(int argc, char *argv[]) {

    if (argc < 2) {
        cerr << "Uso: " << argv[0] << " <archivo_fuente>\n";
        return EXIT_FAILURE;
    }

    ifstream archivo(argv[1]);
    if (!archivo.is_open()) {
        cerr << "Error: no se pudo abrir el archivo \"" << argv[1] << "\"\n";
        return EXIT_FAILURE;
    }

    Contadores c;
    detectarCaracteres(&archivo, &c);
    archivo.close();

    int total = c.caracter + c.cadena + c.includes + c.corrBits +
                c.relComp  + c.igualdad + c.comentario + c.comentMulti +
                c.ternario + c.puntero  + c.accesoDir  + c.sepPara +
                c.finInstr + c.etqcase;

    string linea(58, '-');

    cout << "\n ANALIZADOR LEXICO: " << argv[1] << " ===\n\n";
    cout << left  << setw(36) << "Token"
         << right << setw(8)  << "Cantidad"
         << setw(12) << "Porcentaje\n";
    cout << linea << "\n";

    imprimirFila("Comillas simples (')",           c.caracter,    total);
    imprimirFila("Comillas dobles (\")",           c.cadena,      total);
    imprimirFila("Includes / relacionales (< >)",  c.includes,    total);
    imprimirFila("Corrimiento de bits (<< >>)",     c.corrBits,    total);
    imprimirFila("Relacionales compuestos (<= >=)", c.relComp,     total);
    imprimirFila("Igualdad / desigualdad (== !=)",  c.igualdad,    total);
    imprimirFila("Comentario de linea (//)",         c.comentario,  total);
    imprimirFila("Comentario multilinea (/* */)",    c.comentMulti, total);
    imprimirFila("Operador ternario (?)",            c.ternario,    total);
    imprimirFila("Puntero (->)",                     c.puntero,     total);
    imprimirFila("Acceso directo (.)",               c.accesoDir,   total);
    imprimirFila("Separador de parametros (,)",      c.sepPara,     total);
    imprimirFila("Fin de instruccion (;)",           c.finInstr,    total);
    imprimirFila("Etiqueta / ambito (::)",           c.etqcase,     total);

    cout << linea << "\n";
    cout << left  << setw(36) << "TOTAL"
         << right << setw(8)  << total
         << setw(11) << "  100.00%\n\n";

    return 0;
}
