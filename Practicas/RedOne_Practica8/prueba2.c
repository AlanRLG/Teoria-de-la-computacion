// prueba2_directiva_sin_cerrar.c
// Prueba 2 - Directiva #if sin su #endif
// Error esperado: '#if' en linea 7 sin '#endif' correspondiente

#ifndef MODULO_A
#define MODULO_A

#if VERSION > 1

int funcion(void) {
    int y = 0;

    if (y == 0) {
        y = 1;
    } else {
        y = 2;
    }

    return y;
}

#endif
// El #endif de arriba cierra el #ifndef, pero el #if de la linea 7 queda abierto