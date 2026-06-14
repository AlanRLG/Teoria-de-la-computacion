/*
 * Prueba 7: Manejo de COMENTARIOS.
 * El analizador debe ignorar comentarios de linea y de bloque,
 * pero conservar correctamente el conteo de lineas.
 */

// Comentario de una sola linea
int main() {
    /* Comentario de bloque
       que ocupa varias lineas
       sin afectar el conteo */
    int x = 10; // Comentario al final de la linea
    /* int dentro = 1; */   // Codigo comentado: no debe tokenizarse
    int y = 20;

    /*** Comentario con asteriscos multiples ***/
    if (x < y) {
        // Otro comentario
        x = y; /* asignacion */
    }

    return 0; // Fin del programa
}
