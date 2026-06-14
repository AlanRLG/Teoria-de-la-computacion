/**
 * @file analizador.cpp
 * @brief Analizador Sintactico y Lexico para subconjunto del lenguaje C.
 * @date 13/06/2026
 * @version 1.0
 * @author David Andrawi Perez Marquez
 */

// ============================================================================
//  INCLUSIONES DE BIBLIOTECAS ESTANDAR
// ============================================================================
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <cctype>
#include <stdexcept>

// ============================================================================
//  SECCION 1: ENUMERACIONES Y ESTRUCTURAS
// ============================================================================

/**
 * @brief Categorias de los tokens reconocidos por el Analizador Lexico.
 */
enum class TipoToken {
    Palabra_Clave,    // Reservadas del lenguaje (int, if, while, etc.)
    Identificador,    // Nombres de variables, funciones, etc.
    Constante,        // Numeros, caracteres y cadenas literales
    Puntuador,        // Operadores y signos de puntuacion
    Fin_De_Archivo    // Marca el final del flujo de tokens
};

/**
 * @brief Convierte un TipoToken a su representacion textual en espanol.
 */
std::string tipoTokenATexto(TipoToken tipo) {
    switch (tipo) {
        case TipoToken::Palabra_Clave:  return "Palabra_Clave";
        case TipoToken::Identificador:  return "Identificador";
        case TipoToken::Constante:      return "Constante";
        case TipoToken::Puntuador:      return "Puntuador";
        case TipoToken::Fin_De_Archivo: return "Fin_De_Archivo";
    }
    return "Desconocido";
}

/**
 * @brief Estructura que almacena la informacion de un token detectado.
 */
struct Token {
    TipoToken    tipo;
    std::string  lexema;
    int          linea;
};

// ============================================================================
//  SECCION 2: CLASE LEXER (ANALIZADOR LEXICO)
// ============================================================================

/**
 * @brief Analizador Lexico encargado de transformar el codigo fuente
 *        en una secuencia de tokens, ignorando comentarios y espacios.
 */
class Lexer {
public:
    explicit Lexer(const std::string& fuente)
        : fuente_(fuente), posicion_(0), linea_(1) {}

    /**
     * @brief Ejecuta el analisis lexico completo y devuelve la lista de tokens.
     */
    std::vector<Token> tokenizar() {
        std::vector<Token> tokens;

        while (posicion_ < fuente_.size()) {
            saltarEspaciosYComentarios();
            if (posicion_ >= fuente_.size()) break;

            char actual = fuente_[posicion_];

            if (std::isalpha(static_cast<unsigned char>(actual)) || actual == '_') {
                tokens.push_back(leerIdentificadorOPalabraClave());
            }
            else if (std::isdigit(static_cast<unsigned char>(actual))) {
                tokens.push_back(leerNumero());
            }
            else if (actual == '"') {
                tokens.push_back(leerCadena());
            }
            else if (actual == '\'') {
                tokens.push_back(leerCaracter());
            }
            else {
                tokens.push_back(leerPuntuador());
            }
        }

        tokens.push_back({TipoToken::Fin_De_Archivo, "EOF", linea_});
        return tokens;
    }

    /**
     * @brief Devuelve la lista de errores lexicos detectados durante la tokenizacion.
     */
    const std::vector<std::string>& obtenerErroresLexicos() const {
        return erroresLexicos_;
    }

private:
    std::string              fuente_;
    size_t                   posicion_;
    int                      linea_;
    std::vector<std::string> erroresLexicos_;

    /**
     * @brief Registra un error lexico con su descripcion y linea de origen.
     */
    void registrarErrorLexico(const std::string& descripcion, int linea) {
        std::ostringstream oss;
        oss << "[Error Lexico] Linea " << linea << ": " << descripcion;
        erroresLexicos_.push_back(oss.str());
    }

    // Conjunto de palabras reservadas del subconjunto de C soportado
    const std::unordered_set<std::string> palabrasClave_ = {
        "int", "float", "char", "double", "void",
        "if", "else", "while", "for", "do",
        "return", "break", "continue", "switch", "case", "default"
    };

    /**
     * @brief Salta espacios en blanco, saltos de linea, tabuladores
     *        y los dos estilos de comentarios de C (linea y bloque).
     */
    void saltarEspaciosYComentarios() {
        while (posicion_ < fuente_.size()) {
            char c = fuente_[posicion_];

            if (c == ' ' || c == '\t' || c == '\r') {
                posicion_++;
            }
            else if (c == '\n') {
                linea_++;
                posicion_++;
            }
            // Comentario de una sola linea: //
            else if (c == '/' && posicion_ + 1 < fuente_.size() && fuente_[posicion_ + 1] == '/') {
                while (posicion_ < fuente_.size() && fuente_[posicion_] != '\n') {
                    posicion_++;
                }
            }
            // Comentario multilinea: /* ... */
            else if (c == '/' && posicion_ + 1 < fuente_.size() && fuente_[posicion_ + 1] == '*') {
                int lineaInicioComentario = linea_;
                posicion_ += 2;
                while (posicion_ + 1 < fuente_.size() &&
                       !(fuente_[posicion_] == '*' && fuente_[posicion_ + 1] == '/')) {
                    if (fuente_[posicion_] == '\n') linea_++;
                    posicion_++;
                }
                if (posicion_ + 1 < fuente_.size()) {
                    posicion_ += 2; // Consume el */
                } else {
                    // Comentario de bloque que llega a EOF sin cerrarse.
                    registrarErrorLexico(
                        "Comentario de bloque sin cerrar (falta '*/').",
                        lineaInicioComentario);
                    posicion_ = fuente_.size();
                }
            }
            else {
                break;
            }
        }
    }

    /**
     * @brief Lee un identificador o, si coincide con la tabla, una palabra clave.
     */
    Token leerIdentificadorOPalabraClave() {
        int lineaInicio = linea_;
        size_t inicio = posicion_;

        while (posicion_ < fuente_.size() &&
               (std::isalnum(static_cast<unsigned char>(fuente_[posicion_])) ||
                fuente_[posicion_] == '_')) {
            posicion_++;
        }

        std::string lexema = fuente_.substr(inicio, posicion_ - inicio);
        TipoToken tipo = (palabrasClave_.count(lexema) > 0)
                            ? TipoToken::Palabra_Clave
                            : TipoToken::Identificador;
        return {tipo, lexema, lineaInicio};
    }

    /**
     * @brief Lee una constante numerica entera o de punto flotante.
     *        Detecta dos formas comunes de constantes malformadas:
     *          - Punto decimal sin digitos posteriores (p.ej. "3.").
     *          - Numero seguido directamente por letras o '_' (p.ej. "123abc").
     *        En ambos casos consume el lexema completo invalido y registra
     *        el error lexico, devolviendo un token Constante con dicho lexema.
     */
    Token leerNumero() {
        int lineaInicio = linea_;
        size_t inicio = posicion_;
        bool puntoEncontrado = false;
        bool puntoSinDigitos = false;

        while (posicion_ < fuente_.size() &&
               (std::isdigit(static_cast<unsigned char>(fuente_[posicion_])) ||
                fuente_[posicion_] == '.')) {
            if (fuente_[posicion_] == '.') {
                if (puntoEncontrado) break;
                puntoEncontrado = true;
                // El punto debe estar seguido por al menos un digito decimal.
                if (posicion_ + 1 >= fuente_.size() ||
                    !std::isdigit(static_cast<unsigned char>(fuente_[posicion_ + 1]))) {
                    puntoSinDigitos = true;
                    posicion_++; // Consumir el '.' problematico
                    break;
                }
            }
            posicion_++;
        }

        // Si el numero esta pegado a letras o guion bajo, el lexema es invalido.
        bool letrasPegadas = false;
        if (posicion_ < fuente_.size() &&
            (std::isalpha(static_cast<unsigned char>(fuente_[posicion_])) ||
             fuente_[posicion_] == '_')) {
            letrasPegadas = true;
            while (posicion_ < fuente_.size() &&
                   (std::isalnum(static_cast<unsigned char>(fuente_[posicion_])) ||
                    fuente_[posicion_] == '_')) {
                posicion_++;
            }
        }

        std::string lexema = fuente_.substr(inicio, posicion_ - inicio);

        if (letrasPegadas) {
            registrarErrorLexico(
                "Constante numerica malformada '" + lexema +
                "' (caracteres no validos pegados a la cifra).",
                lineaInicio);
        } else if (puntoSinDigitos) {
            registrarErrorLexico(
                "Constante de punto flotante malformada '" + lexema +
                "' (falta(n) digito(s) decimal(es) tras el punto).",
                lineaInicio);
        }

        return {TipoToken::Constante, lexema, lineaInicio};
    }

    /**
     * @brief Lee una cadena literal encerrada entre comillas dobles.
     */
    Token leerCadena() {
        int lineaInicio = linea_;
        size_t inicio = posicion_;
        posicion_++; // Omitir comilla inicial

        while (posicion_ < fuente_.size() && fuente_[posicion_] != '"') {
            if (fuente_[posicion_] == '\\' && posicion_ + 1 < fuente_.size()) {
                posicion_ += 2;
                continue;
            }
            if (fuente_[posicion_] == '\n') linea_++;
            posicion_++;
        }
        if (posicion_ < fuente_.size()) posicion_++; // Omitir comilla final

        std::string lexema = fuente_.substr(inicio, posicion_ - inicio);
        return {TipoToken::Constante, lexema, lineaInicio};
    }

    /**
     * @brief Lee una constante de tipo caracter encerrada entre comillas simples.
     */
    Token leerCaracter() {
        int lineaInicio = linea_;
        size_t inicio = posicion_;
        posicion_++; // Omitir comilla inicial

        while (posicion_ < fuente_.size() && fuente_[posicion_] != '\'') {
            if (fuente_[posicion_] == '\\' && posicion_ + 1 < fuente_.size()) {
                posicion_ += 2;
                continue;
            }
            posicion_++;
        }
        if (posicion_ < fuente_.size()) posicion_++; // Omitir comilla final

        std::string lexema = fuente_.substr(inicio, posicion_ - inicio);
        return {TipoToken::Constante, lexema, lineaInicio};
    }

    /**
     * @brief Lee un puntuador, operador o simbolo. Reconoce operadores
     *        compuestos como ==, !=, <=, >=, &&, ||, ++, --.
     */
    Token leerPuntuador() {
        int lineaInicio = linea_;
        char actual = fuente_[posicion_];
        char siguiente = (posicion_ + 1 < fuente_.size()) ? fuente_[posicion_ + 1] : '\0';

        std::string lexema(1, actual);

        // Operadores compuestos de dos caracteres
        if ((actual == '=' && siguiente == '=') ||
            (actual == '!' && siguiente == '=') ||
            (actual == '<' && siguiente == '=') ||
            (actual == '>' && siguiente == '=') ||
            (actual == '&' && siguiente == '&') ||
            (actual == '|' && siguiente == '|') ||
            (actual == '+' && siguiente == '+') ||
            (actual == '-' && siguiente == '-') ||
            (actual == '+' && siguiente == '=') ||
            (actual == '-' && siguiente == '=') ||
            (actual == '*' && siguiente == '=') ||
            (actual == '/' && siguiente == '=')) {
            lexema.push_back(siguiente);
            posicion_ += 2;
        } else {
            posicion_++;
        }

        return {TipoToken::Puntuador, lexema, lineaInicio};
    }
};

// ============================================================================
//  SECCION 3: CLASE PARSER (ANALIZADOR SINTACTICO - DESCENSO RECURSIVO)
// ============================================================================

/**
 * @brief Analizador Sintactico por Descenso Recursivo.
 *        Valida declaraciones de variables, estructuras de control
 *        y el balanceo de delimitadores.
 */
class Parser {
public:
    explicit Parser(const std::vector<Token>& tokens)
        : tokens_(tokens), posicion_(0) {}

    /**
     * @brief Ejecuta el analisis sintactico completo del archivo.
     */
    void analizar() {
        parsePrograma();
    }

    /**
     * @brief Devuelve la lista de errores sintacticos detectados.
     */
    const std::vector<std::string>& obtenerErrores() const {
        return errores_;
    }

private:
    const std::vector<Token>& tokens_;
    size_t                    posicion_;
    std::vector<std::string>  errores_;

    // -------- Utilidades del cursor de tokens --------

    const Token& actual() const {
        return tokens_[posicion_];
    }

    const Token& mirar(size_t offset = 1) const {
        size_t idx = posicion_ + offset;
        if (idx >= tokens_.size()) return tokens_.back();
        return tokens_[idx];
    }

    bool finDeTokens() const {
        return actual().tipo == TipoToken::Fin_De_Archivo;
    }

    void avanzar() {
        if (!finDeTokens()) posicion_++;
    }

    bool coincide(TipoToken tipo, const std::string& lexema) const {
        return actual().tipo == tipo && actual().lexema == lexema;
    }

    bool coincideTipo(TipoToken tipo) const {
        return actual().tipo == tipo;
    }

    /**
     * @brief Espera un lexema especifico. Si no aparece, registra error y NO avanza.
     */
    bool esperar(TipoToken tipo, const std::string& lexema) {
        if (actual().tipo == tipo && actual().lexema == lexema) {
            avanzar();
            return true;
        }
        registrarError(lexema, actual().lexema, actual().linea);
        return false;
    }

    void registrarError(const std::string& esperado,
                        const std::string& encontrado,
                        int linea) {
        std::ostringstream oss;
        oss << "[Error de Sintaxis] Linea " << linea
            << ": Se esperaba '" << esperado
            << "' pero se encontro '" << encontrado << "'.";
        errores_.push_back(oss.str());
    }

    void registrarErrorDescriptivo(const std::string& descripcion, int linea) {
        std::ostringstream oss;
        oss << "[Error de Sintaxis] Linea " << linea << ": " << descripcion;
        errores_.push_back(oss.str());
    }

    /**
     * @brief Estrategia de recuperacion consciente de la profundidad de bloques.
     *
     *        Avanza hasta uno de los siguientes puntos de sincronizacion:
     *          - Un ';' situado en el nivel actual (se consume).
     *          - Un '}' que pertenece al bloque envolvente (NO se consume,
     *            para que el parseBloque exterior pueda cerrarlo correctamente).
     *          - Un '{' interno: incrementa la profundidad y, al encontrar su
     *            '}' correspondiente, se consume y se sale de la recuperacion.
     *
     *        Esta logica evita que un error dentro de una sentencia "escape"
     *        del bloque actual y provoque errores fantasma en cascada del tipo
     *        "Delimitador de cierre '}' sin apertura correspondiente".
     */
    void recuperarHastaSincronizador() {
        int profundidad = 0;
        while (!finDeTokens()) {
            const Token& tok = actual();
            if (tok.tipo == TipoToken::Puntuador) {
                if (tok.lexema == "{") {
                    profundidad++;
                    avanzar();
                    continue;
                }
                if (tok.lexema == "}") {
                    if (profundidad == 0) {
                        // El '}' pertenece a un bloque legitimamente abierto
                        // por un parseBloque exterior: se deja sin consumir.
                        return;
                    }
                    profundidad--;
                    avanzar();
                    if (profundidad == 0) {
                        // Cerramos el bloque interno que abrimos durante la
                        // recuperacion; salimos sin avanzar mas.
                        return;
                    }
                    continue;
                }
                if (tok.lexema == ";" && profundidad == 0) {
                    avanzar();
                    return;
                }
            }
            avanzar();
        }
    }

    // -------- Reglas gramaticales --------

    /**
     * @brief Programa -> { Sentencia }* EOF
     */
    void parsePrograma() {
        while (!finDeTokens()) {
            size_t puntoSeguro = posicion_;
            parseSentencia();
            // Salvaguarda: si una sentencia no consumio nada, forzamos avance
            if (posicion_ == puntoSeguro) {
                registrarErrorDescriptivo(
                    "Token inesperado '" + actual().lexema + "'.",
                    actual().linea);
                avanzar();
            }
        }
    }

    /**
     * @brief Sentencia -> Declaracion | DefinicionFuncion | If | While
     *                     | Bloque | Return | ExpresionSentencia
     */
    void parseSentencia() {
        if (esTipoDeDato(actual())) {
            // Distinguir entre declaracion de variable y definicion de funcion
            if (mirar(1).tipo == TipoToken::Identificador &&
                mirar(2).tipo == TipoToken::Puntuador &&
                mirar(2).lexema == "(") {
                parseDefinicionFuncion();
            } else {
                parseDeclaracion();
            }
        }
        else if (coincide(TipoToken::Palabra_Clave, "if")) {
            parseIf();
        }
        else if (coincide(TipoToken::Palabra_Clave, "while")) {
            parseWhile();
        }
        else if (coincide(TipoToken::Palabra_Clave, "return") ||
                 coincide(TipoToken::Palabra_Clave, "break") ||
                 coincide(TipoToken::Palabra_Clave, "continue")) {
            parseSentenciaSalto();
        }
        else if (coincide(TipoToken::Puntuador, "{")) {
            parseBloque();
        }
        else if (coincide(TipoToken::Palabra_Clave, "else")) {
            registrarErrorDescriptivo(
                "'else' sin un 'if' que lo preceda.", actual().linea);
            avanzar();
        }
        else if (coincide(TipoToken::Puntuador, "}") ||
                 coincide(TipoToken::Puntuador, ")") ||
                 coincide(TipoToken::Puntuador, "]")) {
            registrarErrorDescriptivo(
                "Delimitador de cierre '" + actual().lexema +
                "' sin apertura correspondiente.",
                actual().linea);
            avanzar();
        }
        else {
            parseExpresionSentencia();
        }
    }

    bool esTipoDeDato(const Token& t) const {
        if (t.tipo != TipoToken::Palabra_Clave) return false;
        return t.lexema == "int" || t.lexema == "float" ||
               t.lexema == "char" || t.lexema == "double" ||
               t.lexema == "void";
    }

    /**
     * @brief Declaracion -> Tipo Identificador [= Expresion] { , Identificador [= Expresion] }* ;
     */
    void parseDeclaracion() {
        // Consumir el tipo
        avanzar();

        while (true) {
            if (!coincideTipo(TipoToken::Identificador)) {
                registrarError("Identificador", actual().lexema, actual().linea);
                recuperarHastaSincronizador();
                return;
            }
            avanzar(); // Consumir identificador

            // Soporte basico para arreglos: identificador [ Constante ]
            if (coincide(TipoToken::Puntuador, "[")) {
                avanzar();
                if (!coincideTipo(TipoToken::Constante) &&
                    !coincideTipo(TipoToken::Identificador)) {
                    registrarError("Constante o identificador",
                                   actual().lexema, actual().linea);
                    recuperarHastaSincronizador();
                    return;
                }
                avanzar();
                if (!esperar(TipoToken::Puntuador, "]")) {
                    recuperarHastaSincronizador();
                    return;
                }
            }

            // Asignacion opcional
            if (coincide(TipoToken::Puntuador, "=")) {
                avanzar();
                if (!parseExpresion()) {
                    recuperarHastaSincronizador();
                    return;
                }
            }

            if (coincide(TipoToken::Puntuador, ",")) {
                avanzar();
                continue;
            }
            break;
        }

        if (!esperar(TipoToken::Puntuador, ";")) {
            recuperarHastaSincronizador();
        }
    }

    /**
     * @brief DefinicionFuncion -> Tipo Identificador '(' [Parametros] ')' Bloque
     *        Parametros -> Tipo Identificador { ',' Tipo Identificador }*
     */
    void parseDefinicionFuncion() {
        avanzar(); // Tipo de retorno
        avanzar(); // Nombre de la funcion

        if (!esperar(TipoToken::Puntuador, "(")) {
            recuperarHastaSincronizador();
            return;
        }

        // Lista opcional de parametros
        if (!coincide(TipoToken::Puntuador, ")")) {
            while (true) {
                if (!esTipoDeDato(actual())) {
                    registrarError("Tipo de dato", actual().lexema, actual().linea);
                    recuperarHastaSincronizador();
                    return;
                }
                avanzar();
                if (!coincideTipo(TipoToken::Identificador)) {
                    registrarError("Identificador", actual().lexema, actual().linea);
                    recuperarHastaSincronizador();
                    return;
                }
                avanzar();
                if (coincide(TipoToken::Puntuador, ",")) {
                    avanzar();
                    continue;
                }
                break;
            }
        }

        if (!esperar(TipoToken::Puntuador, ")")) {
            recuperarHastaSincronizador();
            return;
        }

        if (coincide(TipoToken::Puntuador, "{")) {
            parseBloque();
        } else if (coincide(TipoToken::Puntuador, ";")) {
            // Prototipo de funcion
            avanzar();
        } else {
            registrarError("{", actual().lexema, actual().linea);
            recuperarHastaSincronizador();
        }
    }

    /**
     * @brief SentenciaSalto -> ('return' [Expresion] | 'break' | 'continue') ';'
     */
    void parseSentenciaSalto() {
        bool esReturn = (actual().lexema == "return");
        avanzar();

        if (esReturn && !coincide(TipoToken::Puntuador, ";")) {
            if (!parseExpresion()) {
                recuperarHastaSincronizador();
                return;
            }
        }

        if (!esperar(TipoToken::Puntuador, ";")) {
            recuperarHastaSincronizador();
        }
    }

    /**
     * @brief If -> 'if' '(' Expresion ')' Sentencia [ 'else' Sentencia ]
     */
    void parseIf() {
        avanzar(); // Consumir 'if'

        if (!esperar(TipoToken::Puntuador, "(")) {
            recuperarHastaSincronizador();
            return;
        }
        if (!parseExpresion()) {
            recuperarHastaSincronizador();
            return;
        }
        if (!esperar(TipoToken::Puntuador, ")")) {
            recuperarHastaSincronizador();
            return;
        }

        parseSentencia();

        if (coincide(TipoToken::Palabra_Clave, "else")) {
            avanzar();
            parseSentencia();
        }
    }

    /**
     * @brief While -> 'while' '(' Expresion ')' Sentencia
     */
    void parseWhile() {
        avanzar(); // Consumir 'while'

        if (!esperar(TipoToken::Puntuador, "(")) {
            recuperarHastaSincronizador();
            return;
        }
        if (!parseExpresion()) {
            recuperarHastaSincronizador();
            return;
        }
        if (!esperar(TipoToken::Puntuador, ")")) {
            recuperarHastaSincronizador();
            return;
        }

        parseSentencia();
    }

    /**
     * @brief Bloque -> '{' { Sentencia }* '}'
     */
    void parseBloque() {
        int lineaApertura = actual().linea;
        avanzar(); // Consumir '{'

        while (!finDeTokens() && !coincide(TipoToken::Puntuador, "}")) {
            size_t puntoSeguro = posicion_;
            parseSentencia();
            if (posicion_ == puntoSeguro) {
                avanzar();
            }
        }

        if (finDeTokens()) {
            registrarErrorDescriptivo(
                "Se esperaba '}' para cerrar el bloque abierto.",
                lineaApertura);
            return;
        }
        avanzar(); // Consumir '}'
    }

    /**
     * @brief ExpresionSentencia -> Expresion ';'
     */
    void parseExpresionSentencia() {
        if (!parseExpresion()) {
            recuperarHastaSincronizador();
            return;
        }
        if (!esperar(TipoToken::Puntuador, ";")) {
            recuperarHastaSincronizador();
        }
    }

    /**
     * @brief Expresion -> ExpresionAsignacion
     *        Reconoce operandos basicos, operadores y subexpresiones.
     *        Devuelve true si se consumio al menos un termino valido.
     */
    bool parseExpresion() {
        if (!parseTermino()) return false;

        // Cadena de operadores binarios
        while (esOperadorBinario(actual())) {
            avanzar();
            if (!parseTermino()) {
                registrarErrorDescriptivo(
                    "Operando faltante despues del operador.",
                    actual().linea);
                return false;
            }
        }
        return true;
    }

    /**
     * @brief Termino -> Constante | Identificador [ '(' Args ')' | '[' Expr ']' ]
     *                  | '(' Expresion ')' | OperadorUnario Termino
     */
    bool parseTermino() {
        // Operador unario
        if (coincide(TipoToken::Puntuador, "!") ||
            coincide(TipoToken::Puntuador, "-") ||
            coincide(TipoToken::Puntuador, "+") ||
            coincide(TipoToken::Puntuador, "++") ||
            coincide(TipoToken::Puntuador, "--")) {
            avanzar();
            return parseTermino();
        }

        if (coincide(TipoToken::Puntuador, "(")) {
            avanzar();
            if (!parseExpresion()) return false;
            if (!esperar(TipoToken::Puntuador, ")")) return false;
            return true;
        }

        if (coincideTipo(TipoToken::Constante)) {
            avanzar();
            return true;
        }

        if (coincideTipo(TipoToken::Identificador)) {
            avanzar();
            // Llamada a funcion
            if (coincide(TipoToken::Puntuador, "(")) {
                avanzar();
                if (!coincide(TipoToken::Puntuador, ")")) {
                    if (!parseExpresion()) return false;
                    while (coincide(TipoToken::Puntuador, ",")) {
                        avanzar();
                        if (!parseExpresion()) return false;
                    }
                }
                if (!esperar(TipoToken::Puntuador, ")")) return false;
            }
            // Acceso a arreglo
            else if (coincide(TipoToken::Puntuador, "[")) {
                avanzar();
                if (!parseExpresion()) return false;
                if (!esperar(TipoToken::Puntuador, "]")) return false;
            }
            // Sufijo ++ o --
            if (coincide(TipoToken::Puntuador, "++") ||
                coincide(TipoToken::Puntuador, "--")) {
                avanzar();
            }
            return true;
        }

        registrarErrorDescriptivo(
            "Se esperaba una expresion, pero se encontro '" +
            actual().lexema + "'.",
            actual().linea);
        return false;
    }

    bool esOperadorBinario(const Token& t) const {
        if (t.tipo != TipoToken::Puntuador) return false;
        static const std::unordered_set<std::string> ops = {
            "+", "-", "*", "/", "%",
            "=", "==", "!=", "<", ">", "<=", ">=",
            "&&", "||", "+=", "-=", "*=", "/="
        };
        return ops.count(t.lexema) > 0;
    }
};

// ============================================================================
//  SECCION 4: FUNCION MAIN (PUNTO DE ENTRADA)
// ============================================================================

/**
 * @brief Lee todo el contenido de un archivo a un string.
 */
std::string leerArchivo(const std::string& ruta) {
    std::ifstream archivo(ruta);
    if (!archivo.is_open()) {
        throw std::runtime_error(
            "No se pudo abrir el archivo: " + ruta);
    }
    std::ostringstream oss;
    oss << archivo.rdbuf();
    return oss.str();
}

/**
 * @brief Imprime el flujo de tokens detectados de forma legible.
 */
void imprimirTokens(const std::vector<Token>& tokens) {
    std::cout << "\n--- FLUJO DE TOKENS DETECTADOS ---\n";
    std::cout << "----------------------------------------------------\n";
    std::cout << "  #   | Linea |       Tipo       | Lexema\n";
    std::cout << "----------------------------------------------------\n";

    int indice = 1;
    for (const auto& tok : tokens) {
        if (tok.tipo == TipoToken::Fin_De_Archivo) continue;
        std::cout << "  " << indice++ << "\t| "
                  << tok.linea << "\t| "
                  << tipoTokenATexto(tok.tipo);
        // Padding visual
        int padding = 14 - static_cast<int>(tipoTokenATexto(tok.tipo).size());
        for (int i = 0; i < padding; ++i) std::cout << ' ';
        std::cout << " | " << tok.lexema << '\n';
    }
    std::cout << "----------------------------------------------------\n";
}

/**
 * @brief Imprime los errores lexicos detectados por el Lexer.
 *        Si no hay errores lexicos la seccion se omite por completo
 *        para no introducir ruido en archivos correctos.
 */
void imprimirErroresLexicos(const std::vector<std::string>& errores) {
    if (errores.empty()) return;
    std::cout << "\n--- ERRORES LEXICOS DETECTADOS ---\n";
    std::cout << "----------------------------------------------------\n";
    std::cout << "Se detectaron " << errores.size()
              << " error(es) lexico(s):\n\n";
    for (const auto& err : errores) {
        std::cout << "  -> " << err << '\n';
    }
    std::cout << "----------------------------------------------------\n";
}

/**
 * @brief Imprime el resultado del analisis sintactico.
 */
void imprimirResultado(const std::vector<std::string>& errores) {
    std::cout << "\n--- RESULTADO DEL ANALISIS SINTACTICO ---\n";
    std::cout << "----------------------------------------------------\n";
    if (errores.empty()) {
        std::cout << "El archivo es sintacticamente CORRECTO.\n";
        std::cout << "No se detectaron errores sintacticos.\n";
    } else {
        std::cout << "Se detectaron " << errores.size()
                  << " error(es) sintactico(s):\n\n";
        for (const auto& err : errores) {
            std::cout << "  -> " << err << '\n';
        }
    }
    std::cout << "----------------------------------------------------\n";
}

/**
 * @brief Punto de entrada del programa.
 */
int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Uso: " << argv[0] << " <archivo.c>\n";
        return 1;
    }

    std::string ruta = argv[1];

    try {
        std::string fuente = leerArchivo(ruta);

        std::cout << "====================================================\n";
        std::cout << "  ANALIZADOR LEXICO Y SINTACTICO PARA LENGUAJE C\n";
        std::cout << "  Archivo analizado: " << ruta << '\n';
        std::cout << "====================================================\n";

        // Fase 1: Analisis Lexico
        Lexer lexer(fuente);
        std::vector<Token> tokens = lexer.tokenizar();
        imprimirTokens(tokens);
        imprimirErroresLexicos(lexer.obtenerErroresLexicos());

        // Fase 2: Analisis Sintactico
        Parser parser(tokens);
        parser.analizar();
        imprimirResultado(parser.obtenerErrores());

    } catch (const std::exception& e) {
        std::cerr << "[Error Fatal] " << e.what() << '\n';
        return 1;
    }

    return 0;
}
