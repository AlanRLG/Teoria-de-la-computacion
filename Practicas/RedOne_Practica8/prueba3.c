// prueba3_else_sin_if.c
// Prueba 3 - else sin if correspondiente en el mismo ambito
// Error esperado: 'else' sin un 'if' correspondiente en el mismo ambito

#include <stdio.h>

int clasificar(int n) {

    if (n > 0) {
        return 1;
    }
    // El if de arriba ya fue "consumido" por este else
    else {
        return -1;
    }

    // Este else no tiene ningun if disponible en este ambito
    else {
        return 0;
    }
}

int main(void) {
    int val = clasificar(5);
    return 0;
}