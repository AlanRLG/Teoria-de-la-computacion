/*
Autores: Red One
    Garcia Ambrosio Aldo
    Leanos Gutierrez Alan Rodrigo
    Perez Marquez David Andrawi
Fecha de creacion: 25/02/26
Descripcion: Programa en ANSI C que realiza operaciones sobre cadenas
             binarias sobre el alfabeto {0,1} usando memoria dinamica.
             Incluye validacion de alfabeto y menu interactivo iterativo.
Version: 2.4
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// DEFINICION DEL ALFABETO
// {0, 1}: unico conjunto de simbolos validos para las cadenas
#define ALFABETO_0 '0'
#define ALFABETO_1 '1'
#define CADENA_VACIA "e" 

// Verifica que cada caracter de la cadena pertenezca a {0,1} 
// Retorna 1 si la cadena es valida, 0 en caso contrario
int esValida(const char *cadena) 
{
    int i;

    if (cadena[0] == '\0') return 0; // rechaza cadena vacia
    for (i = 0; cadena[i] != '\0'; i++) 
    {
        if (cadena[i] != ALFABETO_0 && cadena[i] != ALFABETO_1)
            return 0; // caracter fuera del alfabeto
    }

    return 1;
}

// OPERACIONES

// Cuenta e imprime el numero de caracteres de la cadena
void calcularLongitud(const char *cadena) 
{
    int longitud = 0;

    while (cadena[longitud] != '\0') longitud++; // recorre hasta el fin de cadena
    printf("  |%s| = %d\n", cadena, longitud);
}

// Une c1 y c2 en una nueva cadena reservada dinamicamente
char *concatenarCadenas(const char *c1, const char *c2) 
{
    int len = strlen(c1) + strlen(c2) + 1; // longitud total + terminador
    char *resultado = (char *)malloc(len * sizeof(char)); // reserva memoria exacta

    strcpy(resultado, c1); // copia la primera cadena
    strcat(resultado, c2); // agrega la segunda cadena al final

    return resultado;
}

// Repite la cadena s exactamente n veces; devuelve cadena vacia si n=0
char *potenciarCadena(const char *s, int n) 
{
    int i;
    int lenTotal = (n > 0) ? (int)strlen(s) * n + 1 : 1;  // espacio para n repeticiones; minimo 1 para terminador
    char *resultado = (char *)malloc(lenTotal * sizeof(char)); // reserva memoria 

    resultado[0] = '\0'; // inicia como cadena vacia
    for (i = 0; i < n; i++)
    {
        strcat(resultado, s); // concatena s, n veces
    }

    return resultado;
}

// Construye y devuelve la cadena al reves
char *invertirCadena(const char *s) 
{
    int len = strlen(s), i;
    char *resultado = (char *)malloc((len + 1) * sizeof(char)); // reserva memoria exacta

    for (i = 0; i < len; i++)
    {
        resultado[i] = s[len - i - 1]; // copia de atras hacia adelante
    }

    resultado[len] = '\0';
    return resultado;
}

// Imprime todos los prefijos de s, incluyendo la cadena vacia
void mostrarPrefijos(const char *s) 
{
    int len = strlen(s), i;
    char *prefijo = (char *)malloc((len + 1) * sizeof(char));

    printf("  Prefijos de \"%s\":\n    [1] \"%s\"\n", s, CADENA_VACIA);  // cadena vacia es prefijo
    for (i = 1; i <= len; i++) 
    {
        strncpy(prefijo, s, i); // copia los primeros i caracteres
        prefijo[i] = '\0';
        printf("    [%d] \"%s\"\n", i + 1, prefijo);
    }

    free(prefijo); // libera la memoria
}

// Imprime todos los sufijos de s, incluyendo la cadena vacia
void mostrarSufijos(const char *s) 
{
    int len = strlen(s), i;

    printf("  Sufijos de \"%s\":\n    [1] \"%s\"\n", s, CADENA_VACIA);// cadena vacia es sufijo
    for (i = len - 1; i >= 0; i--)
    {
        printf("    [%d] \"%s\"\n", len - i + 1, s + i); // s+i apunta al inicio de cada sufijo
    }
}

// Imprime todas las subcadenas posibles de s, incluyendo la cadena vacia, sin repetir
void mostrarSubcadenas(const char *s) 
{
    int len = strlen(s), i, j, k, idx = 1, totalPosibles, yaImpresa;
    char **vistas;

    // maximo de subcadenas distintas = len*(len+1)/2 + 1 (cadena vacia)
    totalPosibles = len * (len + 1) / 2 + 1;
    vistas = (char **)malloc(totalPosibles * sizeof(char *));
    for (i = 0; i < totalPosibles; i++) 
    {
        vistas[i] = NULL;
    }

    printf("  Subcadenas de \"%s\":\n    [%d] \"%s\"\n", s, idx++, CADENA_VACIA);
    vistas[0] = (char *)malloc(1 * sizeof(char));   // guarda cadena vacia
    vistas[0][0] = '\0';

    for (i = 0; i < len; i++) 
    {
        for (j = i + 1; j <= len; j++) 
        {
            int subLen = j - i;
            char *sub = (char *)malloc((subLen + 1) * sizeof(char));
            strncpy(sub, s + i, subLen);   // extrae subcadena desde i con longitud subLen
            sub[subLen] = '\0';

            yaImpresa = 0;
            for (k = 0; k < idx - 1; k++)    // revisa si ya fue impresa
            {
                if (vistas[k] != NULL && strcmp(vistas[k], sub) == 0) 
                {
                    yaImpresa = 1;
                    break;
                }
            }

            if (!yaImpresa) 
            {
                printf("    [%d] \"%s\"\n", idx, sub);
                vistas[idx - 1] = (char *)malloc((subLen + 1) * sizeof(char));
                strcpy(vistas[idx - 1], sub);   // registra como ya impresa
                idx++;
            }
            free(sub);
        }
    }

    for (i = 0; i < totalPosibles; i++)
        if (vistas[i] != NULL) free(vistas[i]);   // libera tabla de subcadenas vistas
    free(vistas);
}

// MENU

// Muestra las opciones y retorna la opcion elegida por el usuario
int menu(void) 
{
    int opcion;

    printf("\n======================================\n");
    printf("   Operaciones sobre el Alfabeto\n");
    printf("======================================\n");
    printf("  1. Longitud\n");
    printf("  2. Concatenacion\n");
    printf("  3. Potencia\n");
    printf("  4. Inverso\n");
    printf("  5. Prefijos\n");
    printf("  6. Sufijos\n");
    printf("  7. Subcadenas\n");
    printf("  8. Salir\n");
    printf("--------------------------------------\n");
    printf("  Opcion: ");
    scanf(" %d", &opcion);

    return opcion;
}

// MAIN

int main(void) 
{
    char texto[256];  // temporal para leer desde teclado
    char *cadena1, *cadena2;
    int opcion;

    printf("= Cadenas y Lenguajes =\n");
    printf("  Alfabeto: Sigma = {0, 1}\n\n");

    // Leer cadena 1 con validacion de alfabeto; repite hasta recibir entrada valida
    do {
        printf("Ingrese la cadena 1 (solo '0' y '1'): ");
        scanf("%255s", texto);
        if (!esValida(texto))
            printf("  ERROR: La cadena contiene simbolos fuera de {0,1}. Intente de nuevo.\n");
    } while (!esValida(texto));

    cadena1 = (char *)malloc((strlen(texto) + 1) * sizeof(char));
    strcpy(cadena1, texto);

    // Leer cadena 2 con validacion de alfabeto; repite hasta recibir entrada valida
    do {
        printf("Ingrese la cadena 2 (solo '0' y '1'): ");
        scanf("%255s", texto);
        if (!esValida(texto))
            printf("  ERROR: La cadena contiene simbolos fuera de {0,1}. Intente de nuevo.\n");
    } while (!esValida(texto));

    cadena2 = (char *)malloc((strlen(texto) + 1) * sizeof(char));
    strcpy(cadena2, texto);

    do {
        opcion = menu();

        switch (opcion) 
        {
            case 1:
            {
                printf("\n-- Longitud --\n");
                calcularLongitud(cadena1);
                calcularLongitud(cadena2);
                break;
            }

            case 2: 
            {
                char *resultado = concatenarCadenas(cadena1, cadena2);
                printf("\n-- Concatenacion --\n");
                printf("  %s . %s = %s\n", cadena1, cadena2, resultado);
                free(resultado);   // libera la cadena concatenada
                break;
            }

            case 3: 
            {
                int n, seleccion;
                char *potencia;

                printf("\n-- Potencia --\n");
                do {
                    printf("  Aplicar a cual cadena? (1 o 2): ");
                    scanf(" %d", &seleccion);
                    if (seleccion != 1 && seleccion != 2)
                        printf("  ERROR: opcion invalida, ingrese 1 o 2.\n");
                } while (seleccion != 1 && seleccion != 2);

                do {
                    printf("  Exponente n (n >= 0): ");
                    scanf(" %d", &n);
                    if (n < 0) printf("  ERROR: el exponente debe ser mayor o igual a cero.\n");
                } while (n < 0);

                if (seleccion == 1) 
                {
                    potencia = potenciarCadena(cadena1, n);
                    printf("  (%s)^%d = \"%s\"\n", cadena1, n, n == 0 ? CADENA_VACIA : potencia);
                } else 
                {
                    potencia = potenciarCadena(cadena2, n);
                    printf("  (%s)^%d = \"%s\"\n", cadena2, n, n == 0 ? CADENA_VACIA : potencia);
                }

                free(potencia);   // libera la cadena potenciada
                break;
            }

            case 4: 
            {
                char *inv1 = invertirCadena(cadena1);
                char *inv2 = invertirCadena(cadena2);

                printf("\n-- Inverso --\n");
                printf("  inv(%s) = %s\n", cadena1, inv1);
                printf("  inv(%s) = %s\n", cadena2, inv2);
                
                free(inv1);   // libera el inverso de cadena1
                free(inv2);   // libera el inverso de cadena2
                break;
            }

            case 5:
            {
                printf("\n-- Prefijos --\n");
                mostrarPrefijos(cadena1);
                mostrarPrefijos(cadena2);
                break;
            }

            case 6:
            {
                printf("\n-- Sufijos --\n");
                mostrarSufijos(cadena1);
                mostrarSufijos(cadena2);
                break;
            }

            case 7:
            {
                printf("\n-- Subcadenas --\n");
                mostrarSubcadenas(cadena1);
                mostrarSubcadenas(cadena2);
                break;
            }

            case 8:
            {
                printf("\nSaliendo del programa.\n");
                break;
            }

            default:
                printf("Opcion invalida. Intente de nuevo.\n");
        }

    } while (opcion != 8);

    // Liberar memoria de las cadenas antes de terminar
    free(cadena1);
    free(cadena2);
    return 0;
}