/* prueba2.c - Operaciones con arreglos en C
   Busqueda lineal y ordenamiento burbuja */
#include <stdio.h>
#include <stdlib.h>

void imprimirArreglo(int arr[], int n) {
    int i;
    printf("[");
    for (i = 0; i < n; i++) {
        printf("%d", arr[i]);
        if (i < n - 1) printf(", ");
    }
    printf("]\n");
}

void burbuja(int arr[], int n) {
    int i, j, temp;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

int buscarLineal(int arr[], int n, int objetivo) {
    int i;
    for (i = 0; i < n; i++) {
        if (arr[i] == objetivo) {
            return i;
        }
    }
    return -1;
}

int main() {
    int datos[8] = {42, 17, 85, 3, 66, 29, 11, 54};
    int n = 8;
    printf("Arreglo original: ");
    imprimirArreglo(datos, n);
    burbuja(datos, n);
    printf("Arreglo ordenado: ");
    imprimirArreglo(datos, n);
    int pos = buscarLineal(datos, n, 42);
    if (pos >= 0) {
        printf("Elemento 42 encontrado en posicion [%d]\n", pos);
    }
    return 0;
}
