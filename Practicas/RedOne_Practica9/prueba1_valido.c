/*
 * Prueba 1: Archivo sintacticamente CORRECTO.
 * Cubre declaraciones, asignaciones, if/else, while,
 * arreglos, comentarios y la sentencia return.
 */

// Funcion auxiliar de ejemplo
int suma(int a, int b) {
    return a + b;
}

int main() {
    // Declaraciones basicas de cada tipo
    int    edad   = 21;
    float  altura = 1.78;
    char   inicial = 'D';
    double pi     = 3.141592;

    // Declaracion multiple con y sin inicializacion
    int x, y = 10, z;

    // Arreglo y acceso por indice
    int numeros[5];
    numeros[0] = 42;
    numeros[1] = suma(edad, 4);

    // Estructura if / else con bloque
    if (edad >= 18) {
        x = 1;
    } else {
        x = 0;
    }

    // Estructura if con sentencia simple (sin llaves)
    if (x == 1)
        z = 100;

    // Bucle while
    while (z > 0) {
        z = z - 1;
    }

    /* Operadores compuestos
       y expresiones logicas */
    if ((edad > 10) && (altura < 2.0)) {
        edad = edad + 1;
    }

    return 0;
}
