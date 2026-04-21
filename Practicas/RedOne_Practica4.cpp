/*
 * Titulo    : Practica 4 - Analisis Sintactico de Expresiones Aritmeticas
 * Asignatura: Teoria de la Computacion
 * Alumno    : Garcia Ambrosio Aldo / Leanos Gutierrez Alan Rodrigo /
 *             Perez Marquez David Andrawi
 * Equipo    : Red One  |  Grupo: 4CM5
 * Profesora : Sanchez Garcia Luz Maria
 * Fecha     : Abril 2026
 * Version   : 2.0
 * Descripcion: Valida si una expresion aritmetica pertenece a la
 *              notacion prefija, posfija o infija.
 *              NO convierte entre notaciones ni calcula resultados.
 * Compilar  : g++ -std=c++11 -Wall practica4.cpp -o practica4
 */

#include <iostream>
#include <string>
#include <vector>
#include <cctype>
 
using namespace std;
 
/* ================================================================
 *  HELPERS
 *  Trabajan sobre tokens completos (strings), no sobre chars
 *  sueltos, por lo que soportan operandos multicaracter (ab, x1…)
 * ================================================================ */
 
bool esOperador(const string& t) {
    return t.size() == 1 &&
           (t[0]=='+' || t[0]=='-' || t[0]=='*' || t[0]=='/');
}
 
bool esOperando(const string& t) {
    if (t.empty()) return false;
    for (size_t i = 0; i < t.size(); i++)
        if (!isalnum((unsigned char)t[i])) return false;
    return true;
}
 
/* ================================================================
 *  TOKENIZADOR
 *  Separa la expresion en tokens teniendo en cuenta:
 *    - espacios en blanco (delimitadores)
 *    - parentesis como tokens individuales
 *    - operandos/operadores multicaracter
 * ================================================================ */
 
static vector<string> tokens;
static int posGlobal = 0;
 
void tokenizar(const string& expr) {
    tokens.clear();
    int i = 0, len = (int)expr.size();
    while (i < len) {
        if (isspace((unsigned char)expr[i])) { i++; continue; }
        /* Parentesis: token individual */
        if (expr[i] == '(' || expr[i] == ')') {
            tokens.push_back(string(1, expr[i]));
            i++; continue;
        }
        /* Cualquier otro token (operando, operador, '=') */
        string tok;
        while (i < len && !isspace((unsigned char)expr[i]) &&
               expr[i] != '(' && expr[i] != ')') {
            tok += expr[i++];
        }
        tokens.push_back(tok);
    }
    posGlobal = 0;
}
 
/* Accesores del cursor global */
bool hayToken()           { return posGlobal < (int)tokens.size(); }
const string& actual()    {
    static const string vacio = "";
    return hayToken() ? tokens[posGlobal] : vacio;
}
 
/* ================================================================
 *  NOTACION PREFIJA
 *
 *  Gramatica (descenso recursivo):
 *    prefija -> '=' atomo operando
 *    atomo   -> operando
 *             | operador atomo atomo
 *
 *  El '=' encabeza la expresion, seguido de la expresion completa
 *  y por ultimo el operando que recibe el resultado.
 * ================================================================ */
 
bool atomoPrefijo() {
    if (!hayToken()) return false;
    if (esOperando(actual()))  { posGlobal++; return true; }
    if (esOperador(actual()))  {
        posGlobal++;
        return atomoPrefijo() && atomoPrefijo();
    }
    return false;
}
 
bool validarPrefija(const string& expr) {
    tokenizar(expr);
    int n = (int)tokens.size();
    if (n < 3) return false;
    if (tokens[0] != "=") return false;   /* debe iniciar con '=' */
    posGlobal = 1;
    if (!atomoPrefijo()) return false;   /* expresion bien formada */
    if (!hayToken() || !esOperando(actual())) return false; /* operando final */
    posGlobal++;
    return posGlobal == n;                       /* no deben sobrar tokens */
}
 
/* ================================================================
 *  NOTACION POSFIJA
 *
 *  Algoritmo de contador de pila:
 *    - operando  -> pila++
 *    - operador  -> requiere pila >= 2, pila--
 *    - '=' cierra la expresion; la pila debe quedar en 1
 * ================================================================ */
 
bool validarPosfija(const string& expr) {
    tokenizar(expr);
    int n = (int)tokens.size();
    if (n < 3)  return false;
    if (tokens[n - 1] != "=") return false;  /* debe terminar con '=' */
    int fin = n-1;
    if (esOperando(tokens[fin-1])) fin--;
    
    if (fin < 2) return false;  
    int pila = 0;
    for (int i = 0; i < fin; i++) {
        if      (esOperando(tokens[i])) pila++;
        else if (esOperador(tokens[i])) { if (pila < 2) return false; pila--; }
        else return false;
    }
    return pila == 1;
}
 
/* ================================================================
 *  NOTACION INFIJA
 *
 *  Gramatica con precedencia (descenso recursivo):
 *    asignacion -> expr  '=' operando
 *               | operando '=' expr
 *    expr       -> termino { ('+' | '-') termino  }
 *    termino    -> factor  { ('*' | '/') factor   }
 *    factor     -> '(' expr ')' | operando
 * ================================================================ */
 
bool infExpr();
bool infTermino();
 
bool infFactor() {
    if (!hayToken()) return false;
    if (actual() == "(") {
        posGlobal++;
        if (!infExpr()) return false;
        if (!hayToken() || actual() != ")") return false;
        posGlobal++; return true;
    }
    if (esOperando(actual())) { posGlobal++; return true; }
    return false;
}
 
bool infTermino() {
    if (!infFactor()) return false;
    while (hayToken() && (actual()=="*" || actual()=="/"))
        { posGlobal++; if (!infFactor()) return false; }
    return true;
}
 
bool infExpr() {
    if (!infTermino()) return false;
    while (hayToken() && (actual()=="+" || actual()=="-"))
        { posGlobal++; if (!infTermino()) return false; }
    return true;
}
 
bool validarInfija(const string& expr) {
    tokenizar(expr);
    int n = (int)tokens.size();
    if (n < 3) return false;
 
    /* Caso 1: <expr> = <operando> */
    posGlobal = 0;
    if (infExpr() && hayToken() && actual()=="=") {
        posGlobal++;
        if (hayToken() && esOperando(actual())) {
            posGlobal++;
            if (posGlobal == n) return true;
        }
    }
    /* Caso 2: <operando> = <expr> */
    posGlobal = 0;
    if (esOperando(actual())) {
        posGlobal++;
        if (hayToken() && actual()=="=") {
            posGlobal++;
            if (infExpr() && posGlobal == n) return true;
        }
    }
    return false;
}
 
/* ================================================================
 *  MENU PRINCIPAL
 * ================================================================ */
 int menu() {
    cout << "\n========================================\n";
    cout << "                   MENU \n";
    cout << "========================================\n";
    cout << "1. Verificar Notacion Prefija\n";
    cout << "2. Verificar Notacion Postfija\n";
    cout << "3. Verificar Notacion Infija\n";
    cout << "4. Escribir nueva expresion\n";
    cout << "5. Salir\n";
    cout << "Opcion: ";
    int opcion;
    string lineaOpc;
    getline(cin, lineaOpc);
    try { opcion = stoi(lineaOpc); }
    catch (...) { opcion = -1; } 
    return opcion;
 }


int main() {
    int    opcion;
    string expr;
 
    cout << "Ingrese la expresion: ";
    getline(cin, expr);
 
    do {
        opcion = menu();
       
        bool   valido   = false;
        string notacion;
 
        switch (opcion) {
            case 1: valido = validarPrefija(expr);  notacion = "PREFIJA";  break;
            case 2: valido = validarPosfija(expr);  notacion = "POSFIJA";  break;
            case 3: valido = validarInfija(expr);   notacion = "INFIJA";   break;
            case 4: cout << "Ingrese la nueva expresion: ";
                    getline(cin, expr);
                    continue;
            case 5: cout << "Saliendo...\n"; return 0;
            default: cout << "Opcion invalida. Intente de nuevo.\n"; continue;
        }
 
        cout << "\nResultado: \"" << expr << "\"\n";
        cout << (valido ? "Es valida" : "No es valida")
             << " en notacion " << notacion << ".\n";
 
    } while (true);
 
    cout << "Programa terminado.\n";
    return 0;
}