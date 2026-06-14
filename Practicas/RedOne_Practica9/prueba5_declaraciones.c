/*
 * Prueba 5: Errores en DECLARACIONES de variables.
 * Identificador faltante, asignaciones invalidas y comas mal usadas.
 */
int main() {
    int = 10;              // Falta el identificador
    float ;                // Falta el identificador
    char  c = ;            // Falta el valor en la asignacion
    double d = 3.14
    int a, , b = 1;        // Coma extra entre identificadores
    int x = 5 + ;          // Expresion incompleta
    return 0;
}
