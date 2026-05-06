/* prueba3.c - Manipulacion de cadenas en C
   Uso de string.h y operaciones basicas */
#include <stdio.h>
#include <string.h>
#include <ctype.h>

void aMayusculas(char str[]) {
    int i;
    for (i = 0; str[i] != '\0'; i++) {
        str[i] = toupper((unsigned char)str[i]);
    }
}

int contarVocales(const char str[]) {
    int i, count = 0;
    for (i = 0; str[i] != '\0'; i++) {
        char c = tolower((unsigned char)str[i]);
        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
            count++;
        }
    }
    return count;
}

int esPalindromo(const char str[]) {
    int izq = 0;
    int der = (int)strlen(str) - 1;
    while (izq < der) {
        if (str[izq] != str[der]) return 0;
        izq++;
        der--;
    }
    return 1;
}

int main() {
    char palabra[50] = "escom";
    char palindromo[50] = "reconocer";
    printf("Original:    %s\n", palabra);
    aMayusculas(palabra);
    printf("Mayusculas:  %s\n", palabra);
    printf("Vocales en 'escom': %d\n", contarVocales("escom"));
    printf("'%s' es palindromo: %s\n", palindromo,
           esPalindromo(palindromo) ? "si" : "no");
    return 0;
}
