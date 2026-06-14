/*
 * Prueba 3: Errores de DELIMITADORES desbalanceados.
 * Parentesis, llaves y corchetes mal cerrados o sin apertura.
 */
int main() {
    int x = (5 + 3;          // Falta ')' de cierre
    int arr[10;              // Falta ']' de cierre
    int y = ((2 + 3) * 4;    // Falta ')' de cierre

    if (x > 0 {              // Falta ')' antes de '{'
        x = 1;
    }

    while (x < 10) {
        x = x + 1;
    // Falta '}' de cierre del while

    return 0;
}
