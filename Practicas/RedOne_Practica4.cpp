/*
Practica 4 - Analisis Sintactico de Expresiones Aritmeticas
Teoria de la Computacion
Garcia Ambrosio Aldo
Leanos Gutierrez Alan Rodrigo
Perez Marquez David Andrawi
Red One
Grupo: 4CM5
Version   : 4.0
Descripcion:Recibe una expresion por terminal y determina
            automaticamente si es prefija, posfija o infija.
            NO convierte entre notaciones ni calcula resultados.
Compilar: g++ -std=c++11 -Wall practica4.cpp -o practica4
 */

#include <iostream>
#include <string>
#include <vector>
#include <cctype>

using namespace std;

/* ================================================================
 *  VARIABLES GLOBALES
 *  tokens  : lista de tokens extraidos de la expresion
 *  pos     : posicion actual dentro de la lista de tokens
 * ================================================================ */

vector<string> tokens;
int pos = 0;

/* ================================================================
 *  FUNCIONES DE CLASIFICACION
 *  Determinan si un token es operador u operando.
 * ================================================================ */

// Un operador es exactamente uno de los simbolos: + - * /
bool esOperador(const string& tok) {
    if (tok.size() != 1) return false;
    char c = tok[0];
    return c == '+' || c == '-' || c == '*' || c == '/';
}

// Un operando es una cadena de letras y/o digitos (ej: a, x1, ab)
bool esOperando(const string& tok) {
    if (tok.empty()) return false;
    for (int i = 0; i < (int)tok.size(); i++) {
        if (!isalnum((unsigned char)tok[i])) return false;
    }
    return true;
}

/* ================================================================
 *  TOKENIZADOR
 *  Divide la expresion en partes llamadas tokens:
 *    - Ignora espacios en blanco
 *    - Los parentesis son tokens individuales
 *    - Cualquier otra secuencia continua forma un solo token
 * ================================================================ */

void tokenizar(const string& expr) {
    tokens.clear();
    pos = 0;

    int i = 0;
    int largo = (int)expr.size();

    while (i < largo) {

        // Ignorar espacios
        if (isspace((unsigned char)expr[i])) {
            i++;
            continue;
        }

        // Parentesis: cada uno es su propio token
        if (expr[i] == '(' || expr[i] == ')') {
            tokens.push_back(string(1, expr[i]));
            i++;
            continue;
        }

        // Cualquier otra secuencia sin espacios ni parentesis
        string tok;
        while (i < largo && !isspace((unsigned char)expr[i])
               && expr[i] != '(' && expr[i] != ')') {
            tok += expr[i];
            i++;
        }
        tokens.push_back(tok);
    }
}

// Devuelve true si aun hay tokens por leer
bool hayMasTokens() {
    return pos < (int)tokens.size();
}

// Devuelve el token actual (sin avanzar)
string tokenActual() {
    if (hayMasTokens()) return tokens[pos];
    return "";
}

/* NOTACION PREFIJA 
    Ejemplo valido:   = + a b c
    Ejemplo invalido: + a b = c */

// Intenta leer una sub-expresion prefija desde la posicion actual
bool leerExpresionPrefija() {
    if (!hayMasTokens()) return false;

    // Caso base: si el token actual es un operando, lo consumimos
    if (esOperando(tokenActual())) {
        pos++;
        return true;
    }

    // Caso recursivo: si es operador, debe ir seguido de dos expresiones
    if (esOperador(tokenActual())) {
        pos++;
        bool izquierda = leerExpresionPrefija();
        bool derecha   = leerExpresionPrefija();
        return izquierda && derecha;
    }

    return false;
}

bool validarPrefija(const string& expr) {
    tokenizar(expr);
    int total = (int)tokens.size();

    // Minimo: = op a b  (4 tokens con operador, o = a b para expresion simple)
    if (total < 3) return false;

    // Debe comenzar con '='
    if (tokens[0] != "=") return false;

    // Intentar leer la expresion a partir del token 1
    pos = 1;
    if (!leerExpresionPrefija()) return false;

    // Debe quedar exactamente una variable al final
    if (!hayMasTokens() || !esOperando(tokenActual())) return false;
    pos++;

    // No deben sobrar tokens
    return pos == total;
}

/* NOTACION POSFIJA
    Ejemplo valido:   a b + c =
    Ejemplo invalido: a + b c = */

bool validarPosfija(const string& expr) {
    tokenizar(expr);
    int total = (int)tokens.size();

    // Minimo: a b + =
    if (total < 3) return false;

    // Debe terminar con '='
    if (tokens[total - 1] != "=") return false;

    // Descontar la variable asignada si aparece antes del '='
    int fin = total - 1;
    if (esOperando(tokens[fin - 1])) fin--;

    if (fin < 2) return false;

    // Simular pila
    int pila = 0;
    for (int i = 0; i < fin; i++) {
        if (esOperando(tokens[i])) {
            pila++;
        } else if (esOperador(tokens[i])) {
            if (pila < 2) return false;  // No hay suficientes operandos
            pila--;
        } else {
            return false;  // Token inesperado
        }
    }

    // La pila debe tener exactamente un resultado
    return pila == 1;
}

/* NOTACION INFIJA
    Ejemplo valido:   a + b * c = resultado
    Ejemplo invalido: a + = b c */

bool leerExprInfija();  // Declaracion adelantada

// Lee un factor: un operando o una expresion entre parentesis
bool leerFactorInfijo() {
    if (!hayMasTokens()) return false;

    // Caso: expresion entre parentesis
    if (tokenActual() == "(") {
        pos++;
        if (!leerExprInfija()) return false;
        if (!hayMasTokens() || tokenActual() != ")") return false;
        pos++;
        return true;
    }

    // Caso: operando simple
    if (esOperando(tokenActual())) {
        pos++;
        return true;
    }

    return false;
}

// Lee un termino: factores separados por * o /
bool leerTerminoInfijo() {
    if (!leerFactorInfijo()) return false;

    while (hayMasTokens() && (tokenActual() == "*" || tokenActual() == "/")) {
        pos++;
        if (!leerFactorInfijo()) return false;
    }
    return true;
}

// Lee una expresion completa: terminos separados por + o -
bool leerExprInfija() {
    if (!leerTerminoInfijo()) return false;

    while (hayMasTokens() && (tokenActual() == "+" || tokenActual() == "-")) {
        pos++;
        if (!leerTerminoInfijo()) return false;
    }
    return true;
}

bool validarInfija(const string& expr) {
    tokenizar(expr);
    int total = (int)tokens.size();

    if (total < 3) return false;

    // Caso 1: <expresion> = <variable>
    pos = 0;
    if (leerExprInfija() && hayMasTokens() && tokenActual() == "=") {
        pos++;
        if (hayMasTokens() && esOperando(tokenActual())) {
            pos++;
            if (pos == total) return true;
        }
    }

    // Caso 2: <variable> = <expresion>
    pos = 0;
    if (esOperando(tokenActual())) {
        pos++;
        if (hayMasTokens() && tokenActual() == "=") {
            pos++;
            if (leerExprInfija() && pos == total) return true;
        }
    }

    return false;
}

/* VALIDACION 
    Prueba la expresion en los tres formatos y muestra en cuales es valida.*/

void validarExpresion(const string& expr) {
    bool esPrefija  = validarPrefija(expr);
    bool esPosfija  = validarPosfija(expr);
    bool esInfija   = validarInfija(expr);

    cout << "\nExpresion: \"" << expr << "\"\n";
    cout << "----------------------------------------\n";

    if (!esPrefija && !esPosfija && !esInfija) {
        cout << "La expresion no es valida en ninguna notacion.\n";
        return;
    }

    if (esPrefija)  cout << "Es valida en notacion PREFIJA.\n";
    if (esPosfija)  cout << "Es valida en notacion POSFIJA.\n";
    if (esInfija)   cout << "Es valida en notacion INFIJA.\n";
}

/* PROGRAMA PRINCIPAL */

int main() {
    string expr;
    char continuar;

    do {
        cout << "Ingrese la expresion: ";
        getline(cin, expr);

        validarExpresion(expr);

        cout << "\n¿Desea ingresar otra expresion? (s/n): ";
        cin >> continuar;
        cin.ignore(); // Limpiar el buffer para el siguiente getline

    } while (continuar == 's' || continuar == 'S');

    cout << "Programa finalizado." << endl;

    return 0;
}
