/* prueba1.c - Calculadora basica en C
   Operaciones: suma, resta, multiplicacion, division */
#include <stdio.h>

float suma(float a, float b) {
    return (a + b);
}

float resta(float a, float b) {
    return (a - b);
}

float multiplicacion(float a, float b) {
    return (a * b);
}

float division(float a, float b) {
    if (b == 0) {
        printf("Error: division entre cero\n");
        return 0;
    }
    return (a / b);
}

int main() {
    float x = 10.0;
    float y = 3.0;
    printf("Suma:           %.2f\n", suma(x, y));
    printf("Resta:          %.2f\n", resta(x, y));
    printf("Multiplicacion: %.2f\n", multiplicacion(x, y));
    printf("Division:       %.2f\n", division(x, y));
    return 0;
}
