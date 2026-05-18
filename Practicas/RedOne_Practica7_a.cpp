/*
Practica 7 - Analizador Lexico de Codigo Fuente
Descripcion: Analizador lexico sensible al contexto que detecta y contabiliza
           simbolos en un archivo fuente .c / .cpp. Utiliza una maquina de
           estados (NORMAL, EN_CADENA, EN_CARACTER, EN_COMENT_LINEA,
           EN_COMENT_BLOQUE) para evitar contar tokens dentro de literales o
           comentarios, aplica la regla "maximal munch" para reconocer los
           operadores compuestos antes que los simples, y verifica el balance
           de parentesis mediante una pila.
Version: 4.0
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

/* Estados de la maquina lexica que controla el contexto del flujo. */
enum Estado {
    NORMAL,            /* codigo regular, fuera de literales y comentarios */
    EN_CADENA,         /* dentro de comillas dobles "..."                  */
    EN_CARACTER,       /* dentro de comillas simples '...'                 */
    EN_COMENT_LINEA,   /* dentro de comentario de linea //                 */
    EN_COMENT_BLOQUE   /* dentro de comentario de bloque  / * ... * /      */
};

/* Contadores de cada token reconocido por el analizador. */
typedef struct {
    int caracter;     /* literal de caracter '...'              */
    int cadena;       /* literal de cadena   "..."              */
    int includes;     /* < o > sueltos (relacional / include)   */
    int corrBits;     /* << y >>  (corrimiento de bits)         */
    int relComp;      /* <= y >=  (relacionales compuestos)     */
    int igualdad;     /* == y !=  (igualdad / desigualdad)      */
    int comentario;   /* //      (comentario de linea)          */
    int comentMulti;  /* / * * / (comentario de bloque)         */
    int ternario;     /* ? y :   (operador ternario)            */
    int puntero;      /* ->      (acceso a miembro por puntero) */
    int accesoDir;    /* .       (acceso directo a miembro)     */
    int sepPara;      /* ,       (separador de parametros)      */
    int finInstr;     /* ;       (fin de instruccion)           */
    int etqcase;      /* ::      (resolucion de ambito)         */
    int parIzq;       /* (       (parentesis izquierdo)         */
    int parDer;       /* )       (parentesis derecho)           */
} Contadores;

/*
   Pila estatica para verificar el balance de parentesis.
   Guarda la posicion (numero de caracter) de cada '(' detectado, de modo
   que pueda informarse en que punto del archivo quedo sin cerrar.
 */
#define PILA_MAX 8192

typedef struct {
    int datos[PILA_MAX];
    int tope;
    int desbalance;   /* se activa al hacer pop sobre pila vacia */
} Pila;

static void pilaInit(Pila *p) {
    p->tope = 0;
    p->desbalance = 0;
}

static int pilaVacia(const Pila *p) {
    return p->tope == 0;
}

static void pilaPush(Pila *p, int valor) {
    if (p->tope < PILA_MAX) p->datos[p->tope++] = valor;
}

static void pilaPop(Pila *p) {
    if (pilaVacia(p)) { p->desbalance = 1; return; }
    p->tope--;
}

/* Pone en cero todos los contadores antes de iniciar el analisis. */
static void inicializarContadores(Contadores *c) {
    c->caracter = c->cadena      = c->includes  = c->corrBits = 0;
    c->relComp  = c->igualdad    = c->comentario = c->comentMulti = 0;
    c->ternario = c->puntero     = c->accesoDir  = c->sepPara = 0;
    c->finInstr = c->etqcase     = c->parIzq     = c->parDer = 0;
}

/*
   Recorre el archivo aplicando una maquina de estados sensible al contexto.
   Reglas:
     - Las reglas se evaluan en orden "maximal munch": primero se intenta el
       token de dos caracteres y, si no coincide, se cae al token simple.
     - Los tokens dentro de cadenas o comentarios NO se cuentan; unicamente
       se cuenta el token que abre el contexto (por ejemplo, las comillas
       dobles de una cadena o el // que inicia un comentario de linea).
     - El lookahead se hace con peek(); el segundo caracter se consume con
       un get() adicional unicamente cuando forma parte del token reconocido.
 */
static void analizarArchivo(ifstream &archivo, Contadores *c, Pila *pila) {

    Estado estado = NORMAL;
    char ch;
    int  pos = 0;

    while (archivo.get(ch)) {
        pos++;
        int  vistazo = archivo.peek();
        char next    = (vistazo != EOF) ? (char)vistazo : '\0';

        switch (estado) {

        /* ---------------------------------------------------------------- */
        case NORMAL:
            /* Aperturas de contexto: solo se cuenta el token que abre. */
            if      (ch == '/' && next == '/') { c->comentario++;  archivo.get(ch); estado = EN_COMENT_LINEA;  }
            else if (ch == '/' && next == '*') { c->comentMulti++; archivo.get(ch); estado = EN_COMENT_BLOQUE; }
            else if (ch == '"')                { c->cadena++;                       estado = EN_CADENA;        }
            else if (ch == '\'')               { c->caracter++;                     estado = EN_CARACTER;      }

            /* Operadores compuestos (maximal munch antes que los simples). */
            else if (ch == '<' && next == '<') { c->corrBits++; archivo.get(ch); }
            else if (ch == '>' && next == '>') { c->corrBits++; archivo.get(ch); }
            else if (ch == '<' && next == '=') { c->relComp++;  archivo.get(ch); }
            else if (ch == '>' && next == '=') { c->relComp++;  archivo.get(ch); }
            else if (ch == '=' && next == '=') { c->igualdad++; archivo.get(ch); }
            else if (ch == '!' && next == '=') { c->igualdad++; archivo.get(ch); }
            else if (ch == '-' && next == '>') { c->puntero++;  archivo.get(ch); }
            else if (ch == ':' && next == ':') { c->etqcase++;  archivo.get(ch); }

            /* Tokens simples (fallback). */
            else if (ch == '<' || ch == '>')   c->includes++;
            else if (ch == '?' || ch == ':')   c->ternario++;
            else if (ch == '.')                c->accesoDir++;
            else if (ch == ',')                c->sepPara++;
            else if (ch == ';')                c->finInstr++;
            else if (ch == '(')              { c->parIzq++; pilaPush(pila, pos); }
            else if (ch == ')')              { c->parDer++; pilaPop(pila);       }
            break;

        /* ---------------------------------------------------------------- */
        case EN_CADENA:
            /* La barra invertida absorbe el siguiente caracter (escape). */
            if      (ch == '\\' && next != '\0') archivo.get(ch);
            else if (ch == '"')                  estado = NORMAL;
            break;

        /* ---------------------------------------------------------------- */
        case EN_CARACTER:
            if      (ch == '\\' && next != '\0') archivo.get(ch);
            else if (ch == '\'')                 estado = NORMAL;
            break;

        /* ---------------------------------------------------------------- */
        case EN_COMENT_LINEA:
            if (ch == '\n') estado = NORMAL;
            break;

        /* ---------------------------------------------------------------- */
        case EN_COMENT_BLOQUE:
            if (ch == '*' && next == '/') { archivo.get(ch); estado = NORMAL; }
            break;
        }
    }
}

/* Imprime una fila formateada de la tabla de resultados. */
static void imprimirFila(const string &nombre, int cantidad, int total) {
    double pct = (total > 0) ? (cantidad * 100.0 / total) : 0.0;
    cout << left  << setw(36) << nombre
         << right << setw(8)  << cantidad
         << setw(11) << fixed << setprecision(2) << pct << "%\n";
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
    Pila       pila;
    inicializarContadores(&c);
    pilaInit(&pila);

    analizarArchivo(archivo, &c, &pila);
    archivo.close();

    int total = c.caracter + c.cadena    + c.includes   + c.corrBits +
                c.relComp  + c.igualdad  + c.comentario + c.comentMulti +
                c.ternario + c.puntero   + c.accesoDir  + c.sepPara +
                c.finInstr + c.etqcase   + c.parIzq     + c.parDer;

    string linea(56, '-');

    cout << "\n=== ANALIZADOR LEXICO: " << argv[1] << " ===\n\n";
    cout << left  << setw(36) << "Token"
         << right << setw(8)  << "Cantidad"
         << setw(12) << "Porcentaje\n";
    cout << linea << "\n";

    imprimirFila("Comillas simples (')",            c.caracter,    total);
    imprimirFila("Comillas dobles (\")",            c.cadena,      total);
    imprimirFila("Relacionales / includes (< >)",   c.includes,    total);
    imprimirFila("Corrimiento de bits (<< >>)",     c.corrBits,    total);
    imprimirFila("Relacionales compuestos (<= >=)", c.relComp,     total);
    imprimirFila("Igualdad / desigualdad (== !=)",  c.igualdad,    total);
    imprimirFila("Comentario de linea (//)",        c.comentario,  total);
    imprimirFila("Comentario de bloque (/* */)",    c.comentMulti, total);
    imprimirFila("Ternario (? :)",                  c.ternario,    total);
    imprimirFila("Puntero a miembro (->)",          c.puntero,     total);
    imprimirFila("Acceso directo (.)",              c.accesoDir,   total);
    imprimirFila("Separador de parametros (,)",     c.sepPara,     total);
    imprimirFila("Fin de instruccion (;)",          c.finInstr,    total);
    imprimirFila("Resolucion de ambito (::)",       c.etqcase,     total);
    imprimirFila("Parentesis izquierdo '('",        c.parIzq,      total);
    imprimirFila("Parentesis derecho ')'",          c.parDer,      total);

    cout << linea << "\n";
    cout << left  << setw(36) << "TOTAL"
         << right << setw(8)  << total
         << setw(11) << "100.00" << "%\n\n";

    /* Diagnostico del balance de parentesis. */
    cout << "Balance de parentesis: ";
    if (pila.desbalance) {
        cout << "Error: parentesis ')' sin '(' previo "
             << "(abiertos=" << c.parIzq << ", cerrados=" << c.parDer << ")\n";
    }
    else if (!pilaVacia(&pila)) {
        cout << "Error: " << pila.tope << " parentesis '(' sin cerrar "
             << "(abiertos=" << c.parIzq << ", cerrados=" << c.parDer << ")\n";
    }
    else {
        cout << "OK (abiertos=" << c.parIzq
             << ", cerrados=" << c.parDer << ")\n";
    }

    return 0;
}
