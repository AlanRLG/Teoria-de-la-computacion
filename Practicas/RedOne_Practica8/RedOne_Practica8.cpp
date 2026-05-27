// Practica 8 - Analizador Lexico de Codigo Fuente
//   Lee un archivo .c, detecta y contabiliza simbolos
//   lexicos mostrando la cantidad y porcentaje de aparicion de cada token,
//   y valida que las siguientes estructuras esten bien formadas:
//
//   - Directivas de preprocesador balanceadas: #if / #ifdef / #ifndef <-> #endif
//   - Estructuras if / else (else opcional; las llaves se validan globalmente).
//   - Ciclos do / while (cada do debe cerrarse con su respectivo `while ( ... ) ;`).
//   - Bloques switch / case (debe abrir llaves, contener al menos un case/default y cerrar llaves).
//
//   Fase 1 - Lexeo:      Escaner caracter por caracter que ignora comentarios (linea/bloque)
//                        y literales (cadenas/caracteres), emitiendo solo los tokens estructurales.
//   Fase 2 - Validacion: Recorre el flujo de tokens usando un std::stack para rastrear el
//                        anidamiento de directivas, llaves y estructuras obligatorias.
//
// Compilacion:
//   g++ -Wall -std=c++17 RedOne_Practica8.cpp -o analizador
//
// Uso:
//   ./analizador <archivo.c>
//
// Version: 3.0 (Mayo 2026)
// Autores: Garcia Ambrosio Aldo
//          Leanos Gutierrez Alan Rodrigo
//          Perez Marquez David Andrawi
// Equipo:  Red One

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <optional>
#include <stack>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

// Modelo de tokens
// ---------------------------------------------------------------------
enum class TipoToken {
    HashIf, HashIfdef, HashIfndef, HashEndif,
    If, Else,
    Do, While,
    Switch, Case, Default,
    LlaveIzq, LlaveDer,
    PuntoYComa,
    ParenIzq, ParenDer
};

static std::string_view nombreToken(TipoToken t) {
    switch (t) {
        case TipoToken::HashIf:     return "DIR_IF";
        case TipoToken::HashIfdef:  return "DIR_IFDEF";
        case TipoToken::HashIfndef: return "DIR_IFNDEF";
        case TipoToken::HashEndif:  return "DIR_ENDIF";
        case TipoToken::If:         return "IF";
        case TipoToken::Else:       return "ELSE";
        case TipoToken::Do:         return "DO";
        case TipoToken::While:      return "WHILE";
        case TipoToken::Switch:     return "SWITCH";
        case TipoToken::Case:       return "CASE";
        case TipoToken::Default:    return "DEFAULT";
        case TipoToken::LlaveIzq:   return "LLAVE_IZQ";
        case TipoToken::LlaveDer:   return "LLAVE_DER";
        case TipoToken::PuntoYComa: return "PYC";
        case TipoToken::ParenIzq:   return "PAREN_IZQ";
        case TipoToken::ParenDer:   return "PAREN_DER";
    }
    return "?";
}

struct Token {
    TipoToken   tipo;
    std::string lexema;
    int         linea;
};

// Analizador lexico
// ---------------------------------------------------------------------
class AnalizadorLexico {
public:
    explicit AnalizadorLexico(std::string fuente) : fuente_(std::move(fuente)) {}

    std::vector<Token> tokenizar() {
        while (!finArchivo()) {
            char c = mirar();

            if (c == '\n') { avanzar(); linea_++; inicioDeLinea_ = true; continue; }
            if (std::isspace(static_cast<unsigned char>(c))) { avanzar(); continue; }

            // Comentarios
            if (c == '/' && mirar(1) == '/') { omitirComentarioLinea(); continue; }
            if (c == '/' && mirar(1) == '*') { omitirComentarioBloque(); inicioDeLinea_ = false; continue; }

            // Literales de cadena / caracter
            if (c == '"')  { omitirLiteralCadena();    inicioDeLinea_ = false; continue; }
            if (c == '\'') { omitirLiteralCaracter();  inicioDeLinea_ = false; continue; }

            // Directivas de preprocesador solamente al inicio de la linea
            if (c == '#' && inicioDeLinea_) { manejarPreprocesador(); continue; }

            // Identificadores / palabras reservadas
            if (std::isalpha(static_cast<unsigned char>(c)) || c == '_') {
                escanearIdentificador();
                inicioDeLinea_ = false;
                continue;
            }

            // Signos de puntuacion que nos interesan
            switch (c) {
                case '{': tokens_.push_back({TipoToken::LlaveIzq,   "{", linea_}); avanzar(); inicioDeLinea_ = false; continue;
                case '}': tokens_.push_back({TipoToken::LlaveDer,   "}", linea_}); avanzar(); inicioDeLinea_ = false; continue;
                case ';': tokens_.push_back({TipoToken::PuntoYComa, ";", linea_}); avanzar(); inicioDeLinea_ = false; continue;
                case '(': tokens_.push_back({TipoToken::ParenIzq,   "(", linea_}); avanzar(); inicioDeLinea_ = false; continue;
                case ')': tokens_.push_back({TipoToken::ParenDer,   ")", linea_}); avanzar(); inicioDeLinea_ = false; continue;
                default:  break;
            }

            // Numeros y cualquier otra cosa: se consume y se ignora.
            if (std::isdigit(static_cast<unsigned char>(c))) {
                while (!finArchivo()) {
                    char d = mirar();
                    if (!std::isalnum(static_cast<unsigned char>(d)) && d != '.') break;
                    avanzar();
                }
                inicioDeLinea_ = false;
                continue;
            }

            avanzar();
            inicioDeLinea_ = false;
        }
        return std::move(tokens_);
    }

private:
    // ---- utilidades de bajo nivel ----
    bool finArchivo() const { return posicion_ >= fuente_.size(); }
    char mirar(std::size_t desplazamiento = 0) const {
        return (posicion_ + desplazamiento < fuente_.size()) ? fuente_[posicion_ + desplazamiento] : '\0';
    }
    char avanzar() {
        if (finArchivo()) return '\0';
        return fuente_[posicion_++];
    }

    // ---- omisores ----
    void omitirComentarioLinea() {
        // los caracteres actuales son "//"
        avanzar(); avanzar();
        while (!finArchivo() && mirar() != '\n') avanzar();
        // El salto de linea lo consume el bucle principal.
    }

    void omitirComentarioBloque() {
        avanzar(); avanzar(); // consume "/*"
        while (!finArchivo()) {
            char c = avanzar();
            if (c == '\n') { linea_++; continue; }
            if (c == '*' && mirar() == '/') { avanzar(); return; }
        }
    }

    void omitirLiteralCadena() {
        avanzar(); // comilla de apertura "
        while (!finArchivo()) {
            char c = avanzar();
            if (c == '\\') {
                char e = avanzar();
                if (e == '\n') linea_++;
                continue;
            }
            if (c == '\n') { linea_++; continue; }
            if (c == '"')  return;
        }
    }

    void omitirLiteralCaracter() {
        avanzar(); // comilla de apertura '
        while (!finArchivo()) {
            char c = avanzar();
            if (c == '\\') {
                char e = avanzar();
                if (e == '\n') linea_++;
                continue;
            }
            if (c == '\n') { linea_++; continue; }
            if (c == '\'') return;
        }
    }

    // ---- preprocesador ----
    void manejarPreprocesador() {
        int  inicio = linea_;
        avanzar(); // consume '#'

        // Omitir los espacios entre '#' y el nombre de la directiva.
        while (!finArchivo() && (mirar() == ' ' || mirar() == '\t')) avanzar();

        // Leer el nombre de la directiva.
        std::string nombre;
        while (!finArchivo()) {
            char d = mirar();
            if (std::isalpha(static_cast<unsigned char>(d)) || d == '_') {
                nombre += d;
                avanzar();
            } else break;
        }

        static const std::unordered_map<std::string, TipoToken> kDirectivas = {
            {"if",     TipoToken::HashIf},
            {"ifdef",  TipoToken::HashIfdef},
            {"ifndef", TipoToken::HashIfndef},
            {"endif",  TipoToken::HashEndif},
        };
        if (auto it = kDirectivas.find(nombre); it != kDirectivas.end()) {
            tokens_.push_back({it->second, "#" + nombre, inicio});
        }

        // Consumir el resto de la linea logica (manejar continuaciones de linea).
        char anterior = 0;
        while (!finArchivo()) {
            char d = avanzar();
            if (d == '\n') {
                linea_++;
                if (anterior != '\\') break;
            }
            anterior = d;
        }
        inicioDeLinea_ = true;
    }

    // ---- identificadores ----
    void escanearIdentificador() {
        int inicio = linea_;
        std::string id;
        while (!finArchivo()) {
            char c = mirar();
            if (std::isalnum(static_cast<unsigned char>(c)) || c == '_') { id += c; avanzar(); }
            else break;
        }

        static const std::unordered_map<std::string, TipoToken> kPalabrasReservadas = {
            {"if",      TipoToken::If},
            {"else",    TipoToken::Else},
            {"do",      TipoToken::Do},
            {"while",   TipoToken::While},
            {"switch",  TipoToken::Switch},
            {"case",    TipoToken::Case},
            {"default", TipoToken::Default},
        };
        if (auto it = kPalabrasReservadas.find(id); it != kPalabrasReservadas.end()) {
            tokens_.push_back({it->second, std::move(id), inicio});
        }
        // Los identificadores que no son palabras reservadas se omiten intencionalmente.
    }

private:
    std::string         fuente_;
    std::size_t         posicion_       = 0;
    int                 linea_          = 1;
    bool                inicioDeLinea_  = true;
    std::vector<Token>  tokens_;
};

// Validador
// ---------------------------------------------------------------------
enum class TipoMarco {
    DirIf, DirIfdef, DirIfndef,    // directivas de preprocesador abiertas
    Llave,                          // '{' generica
    LlaveSwitch,                    // '{' que abre el cuerpo de un switch
    Do                              // 'do' en espera de su `while ( ... ) ;`
};

struct Marco {
    TipoMarco tipo;
    int       linea;
    int       casos = 0;            // solo tiene sentido para LlaveSwitch
};

class Validador {
public:
    explicit Validador(const std::vector<Token>& tokens) : tokens_(tokens) {}

    void ejecutar() {
        // Ambito a nivel de archivo para el emparejamiento if/else.
        ambitos_.push_back({0, 0});
        for (std::size_t i = 0; i < tokens_.size(); ++i) {
            const Token& t = tokens_[i];

            // Un switch estaba esperando su '{' de apertura. Cualquier cosa
            // que no sea un parentesis o '{' rompe esa expectativa.
            if (esperaLlaveSwitch_ &&
                t.tipo != TipoToken::ParenIzq &&
                t.tipo != TipoToken::ParenDer &&
                t.tipo != TipoToken::LlaveIzq) {
                reportar(t.linea, "'switch' en la linea " + std::to_string(lineaSwitch_) +
                               " no tiene su '{' de apertura");
                esperaLlaveSwitch_ = false;
            }

            switch (t.tipo) {
                case TipoToken::HashIf:     pila_.push({TipoMarco::DirIf,     t.linea}); break;
                case TipoToken::HashIfdef:  pila_.push({TipoMarco::DirIfdef,  t.linea}); break;
                case TipoToken::HashIfndef: pila_.push({TipoMarco::DirIfndef, t.linea}); break;

                case TipoToken::HashEndif:  manejarEndif(t);    break;

                case TipoToken::If:    ++ambitos_.back().ifs;   break;
                case TipoToken::Else:  manejarElse(t);          break;

                case TipoToken::Switch:
                    esperaLlaveSwitch_ = true;
                    lineaSwitch_       = t.linea;
                    break;

                case TipoToken::Case:
                case TipoToken::Default: manejarCase(t);        break;

                case TipoToken::LlaveIzq: manejarLlaveIzq(t);   break;
                case TipoToken::LlaveDer: manejarLlaveDer(t);   break;

                case TipoToken::Do: pila_.push({TipoMarco::Do, t.linea}); break;
                case TipoToken::While: manejarWhile(i);          break;

                default: break;
            }
        }

        // Cualquier cosa que quede en la pila es una estructura sin cerrar.
        vaciarPila();
    }

    const std::vector<std::string>& errores() const { return errores_; }

private:
    void reportar(int linea, const std::string& mensaje) {
        errores_.push_back("Linea " + std::to_string(linea) + ": " + mensaje);
    }

    void manejarEndif(const Token& t) {
        // Desapilar los marcos anidados no-directiva hasta encontrar la directiva correspondiente.
        std::vector<Marco> desapilados;
        std::optional<Marco> coincidencia;
        while (!pila_.empty()) {
            Marco f = pila_.top(); pila_.pop();
            if (f.tipo == TipoMarco::DirIf ||
                f.tipo == TipoMarco::DirIfdef ||
                f.tipo == TipoMarco::DirIfndef) {
                coincidencia = f;
                break;
            }
            desapilados.push_back(f);
        }
        if (!coincidencia.has_value()) {
            reportar(t.linea, "'#endif' no tiene su '#if/#ifdef/#ifndef' correspondiente");
            // Restaurar los marcos que desapilamos tentativamente.
            for (auto it = desapilados.rbegin(); it != desapilados.rend(); ++it) pila_.push(*it);
            return;
        }
        if (!desapilados.empty()) {
            reportar(t.linea, "'#endif' incorrectamente anidado: directiva abierta en la linea " +
                           std::to_string(coincidencia->linea));
            // Los marcos por encima de la directiva quedaron sin cerrar; los dejamos
            // fuera de la pila para no reportarlos dos veces al final.
        }
    }

    void manejarElse(const Token& t) {
        // 'else' se empareja con un 'if' en el mismo ambito lexico.
        auto& s = ambitos_.back();
        if (s.ifs <= s.elses) {
            reportar(t.linea, "'else' sin un 'if' correspondiente en el mismo ambito");
        } else {
            ++s.elses;
        }
    }

    void manejarCase(const Token& t) {
        // Encontrar el marco switch envolvente mas cercano. std::stack no
        // permite iteracion, asi que desapilamos a un almacenamiento temporal y restauramos.
        std::vector<Marco> temporal;
        bool encontrado = false;
        while (!pila_.empty()) {
            Marco f = pila_.top(); pila_.pop();
            if (f.tipo == TipoMarco::LlaveSwitch) {
                f.casos++;
                temporal.push_back(f);
                encontrado = true;
                break;
            }
            temporal.push_back(f);
        }
        for (auto it = temporal.rbegin(); it != temporal.rend(); ++it) pila_.push(*it);

        if (!encontrado) {
            reportar(t.linea,
                   std::string("'") + (t.tipo == TipoToken::Case ? "case" : "default") +
                   "' fuera de cualquier 'switch'");
        }
    }

    void manejarLlaveIzq(const Token& t) {
        if (esperaLlaveSwitch_) {
            pila_.push({TipoMarco::LlaveSwitch, lineaSwitch_});
            esperaLlaveSwitch_ = false;
        } else {
            pila_.push({TipoMarco::Llave, t.linea});
        }
        // Cada '{' inicia un nuevo ambito para el emparejamiento if/else.
        ambitos_.push_back({0, 0});
    }

    void manejarLlaveDer(const Token& t) {
        if (pila_.empty()) {
            reportar(t.linea, "'}' sin un '{' correspondiente");
            return;
        }
        Marco tope = pila_.top();
        if (tope.tipo == TipoMarco::Llave) {
            pila_.pop();
            cerrarAmbito();
        } else if (tope.tipo == TipoMarco::LlaveSwitch) {
            if (tope.casos == 0) {
                reportar(t.linea, "'switch' en la linea " + std::to_string(tope.linea) +
                               " no tiene 'case' ni 'default'");
            }
            pila_.pop();
            cerrarAmbito();
        } else if (tope.tipo == TipoMarco::Do) {
            reportar(t.linea, "'do' en la linea " + std::to_string(tope.linea) +
                           " no tiene su 'while ( ... ) ;' de cierre");
            pila_.pop();
        } else {
            reportar(t.linea, "'}' inesperado: estructura abierta en la linea " +
                           std::to_string(tope.linea));
            pila_.pop();
        }
    }

    void cerrarAmbito() {
        if (ambitos_.size() > 1) ambitos_.pop_back();
    }

    // Mirar adelante para decidir si un token `while` es el cierre de un do-while.
    // Patron:  WHILE PAREN_IZQ <balanceado...> PAREN_DER PYC
    void manejarWhile(std::size_t i) {
        int parentesis    = 0;
        bool vioParenIzq  = false;
        bool esCierreDo   = false;
        for (std::size_t j = i + 1; j < tokens_.size(); ++j) {
            const Token& w = tokens_[j];
            if (w.tipo == TipoToken::ParenIzq) { ++parentesis; vioParenIzq = true; }
            else if (w.tipo == TipoToken::ParenDer) {
                if (--parentesis == 0) {
                    if (j + 1 < tokens_.size() && tokens_[j + 1].tipo == TipoToken::PuntoYComa) {
                        esCierreDo = true;
                    }
                    break;
                }
            } else if (!vioParenIzq) {
                // Un while que no va seguido de '(' es C malformado; abortamos.
                break;
            }
        }
        if (!esCierreDo) return;

        // Desapilar los marcos hasta encontrar el Do correspondiente.
        std::vector<Marco> desapilados;
        std::optional<Marco> coincidencia;
        while (!pila_.empty()) {
            Marco f = pila_.top(); pila_.pop();
            if (f.tipo == TipoMarco::Do) { coincidencia = f; break; }
            desapilados.push_back(f);
        }
        if (!coincidencia.has_value()) {
            // No hay un do pendiente: es solo un `while (cond);` ordinario.
            for (auto it = desapilados.rbegin(); it != desapilados.rend(); ++it) pila_.push(*it);
            return;
        }
        if (!desapilados.empty()) {
            reportar(tokens_[i].linea,
                   "Estructura sin cerrar antes del 'while' que cierra al 'do' de la linea " +
                   std::to_string(coincidencia->linea));
            // Descartamos los marcos desapilados para no reportarlos dos veces.
        }
    }

    void vaciarPila() {
        while (!pila_.empty()) {
            Marco f = pila_.top(); pila_.pop();
            switch (f.tipo) {
                case TipoMarco::DirIf:
                    reportar(f.linea, "'#if' sin su '#endif' correspondiente");        break;
                case TipoMarco::DirIfdef:
                    reportar(f.linea, "'#ifdef' sin su '#endif' correspondiente");     break;
                case TipoMarco::DirIfndef:
                    reportar(f.linea, "'#ifndef' sin su '#endif' correspondiente");    break;
                case TipoMarco::Llave:
                    reportar(f.linea, "'{' sin su '}' correspondiente");               break;
                case TipoMarco::LlaveSwitch:
                    reportar(f.linea, "'switch' sin su '}' de cierre");                break;
                case TipoMarco::Do:
                    reportar(f.linea, "'do' sin su 'while ( ... ) ;' correspondiente"); break;
            }
        }
    }

private:
    const std::vector<Token>& tokens_;
    std::stack<Marco>         pila_;
    std::vector<std::string>  errores_;

    struct Ambito { int ifs; int elses; };
    std::vector<Ambito> ambitos_;        // una entrada por cada '{' anidado (mas el ambito de archivo)

    bool esperaLlaveSwitch_ = false;
    int  lineaSwitch_       = 0;
};

// Utilidades de reporte
// ---------------------------------------------------------------------
static bool esReportable(TipoToken t) {
    return t != TipoToken::ParenIzq &&
           t != TipoToken::ParenDer &&
           t != TipoToken::PuntoYComa;
}

static void imprimirTokens(const std::vector<Token>& tokens) {
    std::cout << "  TOKENS DETECTADOS (orden cronologico)\n"
              << "\n";
    std::cout << " " << std::left << std::setw(6) << "Linea"
              << "  " << std::setw(12) << "Tipo"
              << "  " << "Lexema" << "\n";
    std::cout << " ------  ------------  --------------------\n";
    for (const auto& t : tokens) {
        if (!esReportable(t.tipo)) continue;
        std::cout << " " << std::left << std::setw(6) << t.linea
                  << "  " << std::setw(12) << nombreToken(t.tipo)
                  << "  " << t.lexema << "\n";
    }
}

static void imprimirResumen(const std::vector<std::string>& errores) {
    std::cout << "\n  RESUMEN DEL ANALISIS\n"
              << "\n";
    if (errores.empty()) {
        std::cout << " Estado: ESTRUCTURA VALIDA\n"
                  << " No se detectaron errores de sintaxis\n";
    } else {
        std::cout << " Estado: ERROR DE SINTAXIS (" << errores.size() << ")\n"
                  << " -------------------------------------------------\n";
        for (const auto& e : errores) std::cout << "  " << e << "\n";
    }
}

// Punto de entrada
// ---------------------------------------------------------------------
int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Uso: " << argv[0] << " <archivo.c>\n";
        return 1;
    }
    std::ifstream entrada(argv[1]);
    if (!entrada) {
        std::cerr << "Error: no se puede abrir '" << argv[1] << "'\n";
        return 1;
    }
    std::string fuente((std::istreambuf_iterator<char>(entrada)),
                        std::istreambuf_iterator<char>());

    std::cout << "Analizando archivo: " << argv[1] << "\n\n";

    AnalizadorLexico lexer(std::move(fuente));
    auto tokens = lexer.tokenizar();
    imprimirTokens(tokens);

    Validador validador(tokens);
    validador.ejecutar();
    imprimirResumen(validador.errores());

    return validador.errores().empty() ? 0 : 1;
}
