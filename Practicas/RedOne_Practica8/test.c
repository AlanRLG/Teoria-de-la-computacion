/* =====================================================================
 * test.c  -  Archivo de entrada de ejemplo para RedOne_Practica8.cpp.
 *
 * Este archivo mezcla deliberadamente construcciones validas e invalidas
 * para que el reporte ejercite todos los caminos de error. Cada caso
 * invalido esta anotado con el error que se espera que emita el analizador.
 * =================================================================== */

#include <stdio.h>

#define MAX 10

/* -------- 1. Directivas de preprocesador balanceadas (valido) --------- */
#ifdef DEBUG
int debug_flag = 1;
#endif

#ifndef HEADER_H
#define HEADER_H
#  if MAX > 0
int positive = 1;
#  endif
#endif

/* -------- Los comentarios y las cadenas NO deben confundir al lexer -- */
/* El siguiente texto dentro de un comentario debe ser ignorado:
 *    if (x) { do { } while(0); }   #endif
 */
const char *trampa = "if (falso) { do {} while (0); } #endif";
const char  ch     = '}';   /* la llave dentro del literal de caracter se ignora */

int main(void) {
    int a = 5;
    int b;

    /* -------- 2. if / else valido ----------------------------------- */
    if (a > 0) {
        b = 1;
    } else {
        b = -1;
    }

    /* if sin else (igualmente valido) */
    if (b) {
        b = 0;
    }

    /* -------- 3. do / while valido ---------------------------------- */
    do {
        a--;
    } while (a > 0);

    /* -------- 4. switch valido -------------------------------------- */
    switch (a) {
        case 0:
            b = 100;
            break;
        case 1:
            b = 200;
            break;
        default:
            b = -1;
    }

    /* -------- 5. Construcciones anidadas validas -------------------- */
    if (a) {
        do {
            if (b) {
                a++;
            } else {
                a--;
            }
        } while (a < 100);
    }

    /* ================================================================
     * CASOS INVALIDOS A CONTINUACION - cada uno debe disparar un error de sintaxis.
     * Comente un bloque para confirmar que aparece el error correspondiente.
     * ============================================================== */

    /* (E1) else sin un 'if' en su ambito */
    {
        else {                     /* esperado: 'else' sin 'if' */
            b = 0;
        }
    }

    /* (E2) do sin un while ( ... ) ; de cierre */
    do {
        a++;
    }                              /* esperado: 'do' sin ... */

    /* (E3) switch sin case / default */
    switch (a) {                   /* esperado: switch sin 'case' */
        b = 0;
    }

    /* (E4) Llaves desbalanceadas: '}' de mas */
    {
        b = 1;
    }
    }                              /* esperado: '}' sin '{' */

    /* (E5) Llaves desbalanceadas: falta '}' (abre un marco que nunca se cierra) */
    {
        b = 2;

    return 0;
}

/* (E6) #endif sin #if */
#endif                              /* esperado: '#endif' sin ... */

/* (E7) #if sin #endif */
#ifdef NEVER_CLOSED                 /* esperado: '#ifdef' sin '#endif' */
int dangling = 1;
