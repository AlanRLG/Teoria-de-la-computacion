/*
Practica 4 - Analisis Sintactico y Conversion de Expresiones Aritmeticas
Teoria de la Computacion
Garcia Ambrosio Aldo
Leaños Gutierrez Alan Rodrigo
Pérez Marquez David Andrawi
Red One
Grupo: 4CM5
Version   : 5.3
Descripcion: Recibe una expresion por terminal, determina automaticamente
             si es prefija, posfija o infija, y la convierte a los otros
             dos formatos restantes usando pilas (stacks).
             Cambios respecto a 5.2:
             - El '=' se trata como operador binario de precedencia minima
               y asociatividad derecha, por lo que los operandos conservan
               su orden natural en prefija y posfija:
                 x = y + z  ->  Prefija: = x + y z
                             ->  Posfija: x y z + =
             - El tokenizador y validadores son los mismos de la v5.2.
Compilar: g++ -std=c++11 -Wall practica4.cpp -o practica4
*/

#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <cctype>
#include <algorithm>

using namespace std;

/* ================================================================
 *  VARIABLES GLOBALES
 * ================================================================ */

vector<string> tokens;
int pos = 0;

/* ================================================================
 *  FUNCIONES DE CLASIFICACION
 * ================================================================ */

// Operadores aritmeticos (sin incluir '=')
bool esOperadorAritmetico(const string& tok) {
    if (tok.size() != 1) return false;
    char c = tok[0];
    return c == '+' || c == '-' || c == '*' || c == '/';
}

// Cualquier operador, incluyendo '='
bool esOperador(const string& tok) {
    return esOperadorAritmetico(tok) || tok == "=";
}

// Operando: secuencia alfanumerica (letras y/o digitos)
bool esOperando(const string& tok) {
    if (tok.empty()) return false;
    for (unsigned char c : tok)
        if (!isalnum(c)) return false;
    return true;
}

/* ================================================================
 *  PRECEDENCIA Y ASOCIATIVIDAD
 *
 *  El '=' tiene la precedencia mas baja de todos (0) y asociatividad
 *  DERECHA, lo que significa que en Shunting-Yard se desapila solo
 *  cuando el operador del tope tiene precedencia ESTRICTAMENTE mayor.
 *
 *  Los operadores aritmeticos tienen asociatividad IZQUIERDA (normal):
 *  se desapila cuando el tope tiene precedencia >= actual.
 * ================================================================ */

int precedencia(const string& op) {
    if (op == "=")             return 0;  // minima: raiz del arbol
    if (op == "+" || op == "-") return 1;
    if (op == "*" || op == "/") return 2;
    return -1;
}

// Devuelve true si el operador tiene asociatividad izquierda
bool esAsocIzquierda(const string& op) {
    return op != "=";  // solo '=' es asociativo a la derecha
}

/* ================================================================
 *  TOKENIZADOR
 *  - Espacios ignorados.
 *  - Parentesis, operadores y '=' son tokens individuales.
 *  - Operandos alfanumericos multi-caracter se agrupan.
 *  - Funciona aunque no haya espacios: (a+b)*c
 * ================================================================ */

void tokenizar(const string& expr) {
    tokens.clear();
    pos = 0;

    int i = 0;
    int largo = (int)expr.size();

    while (i < largo) {
        unsigned char c = expr[i];

        if (isspace(c)) { i++; continue; }

        if (c == '(' || c == ')' || c == '=' ||
            c == '+' || c == '-' || c == '*' || c == '/') {
            tokens.push_back(string(1, expr[i++]));
            continue;
        }

        if (isalnum(c)) {
            string tok;
            while (i < largo && isalnum((unsigned char)expr[i]))
                tok += expr[i++];
            tokens.push_back(tok);
            continue;
        }

        // Caracter desconocido: pasa como token para fallar en la validacion
        tokens.push_back(string(1, expr[i++]));
    }
}

bool hayMasTokens() { return pos < (int)tokens.size(); }
string tokenActual() { return hayMasTokens() ? tokens[pos] : ""; }

/* ================================================================
 *  VALIDADORES
 *  Cada uno devuelve true/false. El rango de la expresion pura
 *  ya no necesita calcularse porque el '=' se incluye en la
 *  conversion como operador normal.
 * ================================================================ */

/* -- PREFIJA --
   Gramatica: expr_pre ::= operando | operador expr_pre expr_pre
   Con '=' admitido: = expr_pre expr_pre  (p. ej. = x + y z)
   Sin '=': + a b  */

bool leerExpresionPrefija() {
    if (!hayMasTokens()) return false;
    if (esOperando(tokenActual()))  { pos++; return true; }
    if (esOperador(tokenActual()))  {
        pos++;
        return leerExpresionPrefija() && leerExpresionPrefija();
    }
    return false;
}

bool validarPrefija(const string& expr) {
    tokenizar(expr);
    int total = (int)tokens.size();
    if (total < 2) return false;

    // La expresion prefija empieza por un operador
    if (!esOperador(tokens[0])) return false;

    pos = 0;
    if (!leerExpresionPrefija()) return false;
    if (pos != total) return false;  // no deben sobrar tokens

    // Debe tener al menos un operador aritmetico (evita falsos positivos)
    for (auto& t : tokens)
        if (esOperadorAritmetico(t)) return true;
    return false;
}

/* -- POSFIJA --
   Simulacion de pila sobre TODOS los tokens, incluyendo '='.
   El '=' se trata como un operador binario normal (consume dos operandos).
   Sin '=': a b +    Con '=': x y z + =    o    a b + c = */

bool validarPosfija(const string& expr) {
    tokenizar(expr);
    int total = (int)tokens.size();
    if (total < 2) return false;

    // No puede empezar por un operador (eso es prefija)
    if (esOperador(tokens[0])) return false;

    // Debe terminar en operador ('+', '-', '*', '/' o '=')
    if (!esOperador(tokens[total - 1])) return false;

    // No debe contener parentesis
    for (auto& t : tokens)
        if (t == "(" || t == ")") return false;

    // Solo operandos y operadores en toda la cadena
    for (auto& t : tokens)
        if (!esOperando(t) && !esOperador(t)) return false;

    // Simulacion de pila: '=' cuenta como operador binario
    int pila = 0;
    for (auto& t : tokens) {
        if      (esOperando(t)) { pila++; }
        else if (esOperador(t)) { if (pila < 2) return false; pila--; }
    }
    if (pila != 1) return false;

    // Debe tener al menos un operador aritmetico (evita que "a =" sea valida)
    for (auto& t : tokens)
        if (esOperadorAritmetico(t)) return true;
    return false;
}

/* -- INFIJA --
   Gramatica con precedencia y parentesis.
   El '=' se acepta como operador de menor precedencia dentro de la
   expresion, lo que permite: x = y + z, a + b = c, x = y = z. */

bool leerExprInfija();

bool leerFactorInfijo() {
    if (!hayMasTokens()) return false;
    if (tokenActual() == "(") {
        pos++;
        if (!leerExprInfija()) return false;
        if (!hayMasTokens() || tokenActual() != ")") return false;
        pos++;
        return true;
    }
    if (esOperando(tokenActual())) { pos++; return true; }
    return false;
}

bool leerTerminoInfijo() {
    if (!leerFactorInfijo()) return false;
    while (hayMasTokens() && (tokenActual() == "*" || tokenActual() == "/")) {
        pos++;
        if (!leerFactorInfijo()) return false;
    }
    return true;
}

bool leerExprAritmeticaInfija() {
    if (!leerTerminoInfijo()) return false;
    while (hayMasTokens() && (tokenActual() == "+" || tokenActual() == "-")) {
        pos++;
        if (!leerTerminoInfijo()) return false;
    }
    return true;
}

// Expresion infija completa: admite uno o mas '=' encadenados
bool leerExprInfija() {
    if (!leerExprAritmeticaInfija()) return false;
    while (hayMasTokens() && tokenActual() == "=") {
        pos++;
        if (!leerExprAritmeticaInfija()) return false;
    }
    return true;
}

bool validarInfija(const string& expr) {
    tokenizar(expr);
    int total = (int)tokens.size();
    if (total < 1) return false;

    // No debe empezar por operador (salvo '(' que es valido en infija)
    if (esOperadorAritmetico(tokens[0]) || tokens[0] == "=") return false;

    pos = 0;
    if (!leerExprInfija()) return false;
    if (pos != total) return false;

    // Debe tener al menos un operador (aritmetico o '=')
    for (auto& t : tokens)
        if (esOperador(t)) return true;
    return false;
}

/* ================================================================
 *  NUCLEO DE CONVERSION: Shunting-Yard generalizado
 *
 *  Soporta el operador '=' con precedencia 0 y asociatividad derecha.
 *
 *  Para posfija  (izq_a_der = true):
 *    - Operadores con igual precedencia y asoc. izquierda se desapilan.
 *    - Operadores con asoc. derecha (solo '=') nunca se desapilan por
 *      igualdad de precedencia.
 *
 *  Para prefija (izq_a_der = false, sobre cadena invertida):
 *    - Solo se desapila cuando la precedencia del tope es ESTRICTAMENTE
 *      mayor (equivalente a asoc. izquierda sobre el resultado invertido).
 * ================================================================ */

vector<string> shuntingYard(const vector<string>& toks, bool izq_a_der) {
    stack<string> pilaOps;
    vector<string> salida;

    for (const string& tok : toks) {
        if (esOperando(tok)) {
            salida.push_back(tok);

        } else if (esOperador(tok)) {
            while (!pilaOps.empty() && esOperador(pilaOps.top())
                   && pilaOps.top() != "(") {
                int pTop = precedencia(pilaOps.top());
                int pCur = precedencia(tok);
                bool desapilar;
                if (izq_a_der) {
                    // Posfija: desapilar si tope tiene mayor precedencia,
                    // O igual precedencia Y el operador actual es asoc. izquierda
                    desapilar = (pTop > pCur) ||
                                (pTop == pCur && esAsocIzquierda(tok));
                } else {
                    // Prefija (cadena invertida): desapilar solo si estrictamente mayor
                    desapilar = (pTop > pCur);
                }
                if (desapilar) { salida.push_back(pilaOps.top()); pilaOps.pop(); }
                else break;
            }
            pilaOps.push(tok);

        } else if (tok == "(") {
            pilaOps.push(tok);

        } else if (tok == ")") {
            while (!pilaOps.empty() && pilaOps.top() != "(") {
                salida.push_back(pilaOps.top());
                pilaOps.pop();
            }
            if (!pilaOps.empty()) pilaOps.pop(); // descartar '('
        }
    }

    while (!pilaOps.empty()) {
        salida.push_back(pilaOps.top());
        pilaOps.pop();
    }
    return salida;
}

/* ================================================================
 *  CONVERSIONES DESDE INFIJA
 * ================================================================ */

// Infija -> Posfija
vector<string> infijaAPosfija(const vector<string>& toks) {
    return shuntingYard(toks, true);
}

// Infija -> Prefija: invertir + Shunting-Yard con asoc. derecha + invertir
vector<string> infijaAPrefija(const vector<string>& toks) {
    vector<string> invertidos(toks.rbegin(), toks.rend());
    for (string& t : invertidos) {
        if (t == "(") t = ")";
        else if (t == ")") t = "(";
    }
    vector<string> temp = shuntingYard(invertidos, false);
    reverse(temp.begin(), temp.end());
    return temp;
}

/* ================================================================
 *  CONVERSIONES DESDE POSFIJA
 *  El '=' en la entrada se incluye como operador en la pila
 *  y se procesa igual que cualquier operador binario.
 * ================================================================ */

// Posfija -> Infija: pila de strings, cada operador combina dos operandos
vector<string> posfija_a_infija(const vector<string>& toks) {
    stack<string> pila;
    for (const string& tok : toks) {
        if (esOperando(tok)) {
            pila.push(tok);
        } else if (esOperador(tok)) {
            string der = pila.top(); pila.pop();
            string izq = pila.top(); pila.pop();
            pila.push("( " + izq + " " + tok + " " + der + " )");
        }
    }
    tokenizar(pila.top());
    return tokens;
}

// Posfija -> Prefija: pila de strings, cada operador forma "op izq der"
vector<string> posfija_a_prefija(const vector<string>& toks) {
    stack<string> pila;
    for (const string& tok : toks) {
        if (esOperando(tok)) {
            pila.push(tok);
        } else if (esOperador(tok)) {
            string der = pila.top(); pila.pop();
            string izq = pila.top(); pila.pop();
            pila.push(tok + " " + izq + " " + der);
        }
    }
    tokenizar(pila.top());
    return tokens;
}

/* ================================================================
 *  CONVERSIONES DESDE PREFIJA
 *  La expresion prefija incluye '=' como primer token si hay asignacion.
 *  Se recorre de derecha a izquierda; cada operador combina dos resultados.
 * ================================================================ */

// Prefija -> Posfija: recorre derecha a izquierda
vector<string> prefija_a_posfija(const vector<string>& toks) {
    stack<string> pila;
    for (int i = (int)toks.size() - 1; i >= 0; i--) {
        const string& tok = toks[i];
        if (esOperando(tok)) {
            pila.push(tok);
        } else if (esOperador(tok)) {
            string izq = pila.top(); pila.pop();
            string der = pila.top(); pila.pop();
            pila.push(izq + " " + der + " " + tok);
        }
    }
    tokenizar(pila.top());
    return tokens;
}

// Prefija -> Infija: recorre derecha a izquierda, forma "( izq op der )"
vector<string> prefija_a_infija(const vector<string>& toks) {
    stack<string> pila;
    for (int i = (int)toks.size() - 1; i >= 0; i--) {
        const string& tok = toks[i];
        if (esOperando(tok)) {
            pila.push(tok);
        } else if (esOperador(tok)) {
            string izq = pila.top(); pila.pop();
            string der = pila.top(); pila.pop();
            pila.push("( " + izq + " " + tok + " " + der + " )");
        }
    }
    tokenizar(pila.top());
    return tokens;
}

/* ================================================================
 *  HELPER: une un vector de tokens en una cadena legible
 * ================================================================ */

string tokensAString(const vector<string>& toks) {
    string r;
    for (int i = 0; i < (int)toks.size(); i++) {
        if (i > 0) r += " ";
        r += toks[i];
    }
    return r;
}

/* ================================================================
 *  LOGICA PRINCIPAL DE PROCESAMIENTO
 *  Detecta la notacion, convierte a las otras dos y muestra el resultado.
 *  El '=' se incluye en los tokens de conversion como operador normal.
 * ================================================================ */

void procesarExpresion(const string& expr) {
    bool esPre = validarPrefija(expr);
    bool esPos = validarPosfija(expr);
    bool esInf = validarInfija(expr);

    cout << "\nExpresion: \"" << expr << "\"\n";
    cout << "----------------------------------------\n";

    if (!esPre && !esPos && !esInf) {
        cout << "La expresion no es valida en ninguna notacion.\n";
        return;
    }

    // Tomar la notacion con mayor prioridad (prefija > posfija > infija)
    string tipo = esPre ? "PREFIJA" : (esPos ? "POSFIJA" : "INFIJA");
    cout << "Notacion detectada: " << tipo << "\n\n";

    // Tokenizar de nuevo para obtener la lista limpia
    tokenizar(expr);
    vector<string> toks = tokens;   // copia completa (con '=' si existe)

    if (tipo == "INFIJA") {
        cout << "-> Conversion a PREFIJA : "
             << tokensAString(infijaAPrefija(toks)) << "\n";
        cout << "-> Conversion a POSFIJA : "
             << tokensAString(infijaAPosfija(toks)) << "\n";
    } else if (tipo == "PREFIJA") {
        cout << "-> Conversion a INFIJA  : "
             << tokensAString(prefija_a_infija(toks)) << "\n";
        cout << "-> Conversion a POSFIJA : "
             << tokensAString(prefija_a_posfija(toks)) << "\n";
    } else { // POSFIJA
        cout << "-> Conversion a INFIJA  : "
             << tokensAString(posfija_a_infija(toks)) << "\n";
        cout << "-> Conversion a PREFIJA : "
             << tokensAString(posfija_a_prefija(toks)) << "\n";
    }
}

/* ================================================================
 *  PROGRAMA PRINCIPAL
 * ================================================================ */

int main() {
    string expr;
    char continuar;

    cout << "=== Analizador y Convertidor de Expresiones Aritmeticas ===\n";
    cout << "Formatos aceptados:\n";
    cout << "  Infija  : x = y + z  |  a + b = c  |  (a+b)*c  |  10+20\n";
    cout << "  Prefija : = x + y z  |  + a b\n";
    cout << "  Posfija : x y z + =  |  a b +\n\n";

    do {
        cout << "Ingrese la expresion: ";
        getline(cin, expr);

        procesarExpresion(expr);

        cout << "\n¿Desea ingresar otra expresion? (S/N): ";
        cin >> continuar;
        cin.ignore();

    } while (continuar == 's' || continuar == 'S');

    cout << "\nPrograma finalizado." << endl;
    return 0;
}
