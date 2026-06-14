/*
 * Prueba 2: Errores de PUNTO Y COMA faltante.
 * Verifica que el parser detecte el ';' ausente
 * tras declaraciones y expresiones.
 */
int main() {
    int a = 5      // Falta ';' al final
    int b = 10;
    float c = 2.5  // Falta ';' al final
    c = c + 1;
    b = b * 2      // Falta ';' al final
    return 0;
}
