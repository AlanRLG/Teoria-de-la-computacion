// prueba1_valido.c
// Prueba 1 - Estructura completamente valida
// Todas las construcciones estan correctamente formadas.

#ifndef MI_CABECERA
#define MI_CABECERA

#ifdef DEBUG
    #if VERSION > 2
        // bloque anidado
    #endif
#endif

int main(void) {

    int x = 5;

    /* Estructura if / else */
    if (x > 3) {
        x = x + 1;
    } else {
        x = x - 1;
    }

    /* Ciclo do / while */
    do {
        x--;
    } while (x > 0);

    /* switch con case y default */
    switch (x) {
        case 0:
            x = 10;
            break;
        case 1:
            x = 20;
            break;
        default:
            x = 0;
            break;
    }

    return 0;
}

#endif