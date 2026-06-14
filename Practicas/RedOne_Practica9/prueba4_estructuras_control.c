/*
 * Prueba 4: Errores en ESTRUCTURAS DE CONTROL.
 * if, else y while mal formados.
 */
int main() {
    int x = 5;

    // 'if' sin parentesis de apertura
    if x > 0) {
        x = 1;
    }

    // 'while' sin parentesis
    while x < 10 {
        x = x + 1;
    }

    // 'else' suelto sin 'if' previo
    else {
        x = 0;
    }

    // Condicion vacia en if
    if () {
        x = 2;
    }

    return 0;
}
