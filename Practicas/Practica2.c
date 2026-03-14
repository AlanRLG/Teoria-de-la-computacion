/*
Autores: Red One
    Garcia Ambrosio Aldo
    Leaños Gutierrez Alan Rodrigo
    Perez Marquez David Andrawi
Fecha de creacion:
Descripcion: Programa en ANSI C que realiza operaciones sobre cadenas
             binarias sobre el alfabeto {0,1} usando memoria dinamica.
             Incluye validacion de alfabeto y menu interactivo iterativo.
Version: 3.2
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// DEFINICION DEL ALFABETO
// {0, 1}: unico conjunto de simbolos validos para las cadenas
#define ALFABETO_0 '0'
#define ALFABETO_1 '1'
#define CADENA_VACIA "e"
// L+ = L^1 U L^2 U L^3
#define LIMITE 3

// Estructura para definir los lenguajes
typedef struct
{
    char *palabras[200]; // Capacidad para 200 cadenas (ajustable)
    int cantidad;        // Número actual de palabras en el lenguaje
} Lenguaje;

// Verifica que cada caracter de la cadena pertenezca a {0,1}
// Retorna 1 si la cadena es valida, 0 en caso contrario
int esValida(const char *cadena)
{
    // 1. Si la cadena es exactamente igual a tu macro de cadena vacía, es válida.
    if (strcmp(cadena, CADENA_VACIA) == 0)
        return 1;

    // 2. Si la cadena está realmente vacía (longitud 0), también es válida en teoría de lenguajes.
    if (cadena[0] == '\0')
        return 1;

    // 3. Verificación de los símbolos del alfabeto
    for (int i = 0; cadena[i] != '\0'; i++)
    {
        if (cadena[i] != ALFABETO_0 && cadena[i] != ALFABETO_1)
            return 0; // Carácter no permitido
    }

    return 1;
}

// Verifica si una cadena ya existe dentro de un lenguaje.
int existeEnLenguaje(Lenguaje leng, const char *cadena)
{
    for (int i = 0; i < leng.cantidad; i++)
    {
        if (strcmp(leng.palabras[i], cadena) == 0)
            return 1; // Coincidencia encontrada
    }
    return 0; // La cadena no pertenece al conjunto actual
}

// Union de dos lenguajes: resultado = A ∪ B
// Retorna un nuevo lenguaje con todas las palabras de A y B sin repetir
Lenguaje unionC(Lenguaje A, Lenguaje B)
{
    Lenguaje resultado;
    resultado.cantidad = 0;

    // 1. Agregar todas las palabras de A al resultado
    for (int i = 0; i < A.cantidad; i++)
    {
        if (!existeEnLenguaje(resultado, A.palabras[i]))
            resultado.palabras[resultado.cantidad++] = A.palabras[i];
    }

    // 2. Agregar palabras de B que no estén ya en el resultado
    //    Esto evita duplicados cuando A y B comparten palabras
    for (int i = 0; i < B.cantidad; i++)
    {
        if (!existeEnLenguaje(resultado, B.palabras[i]))
            resultado.palabras[resultado.cantidad++] = B.palabras[i];
    }

    return resultado;
}

// Interseccion de dos lenguajes: resultado = A ∩ B
// Retorna un nuevo lenguaje con las palabras que pertenecen a AMBOS lenguajes
Lenguaje interseccionCadenas(Lenguaje A, Lenguaje B)
{
    Lenguaje resultado;
    resultado.cantidad = 0;

    // Recorremos A y verificamos si cada palabra tambien existe en B
    // Solo se agrega si cumple la condicion de pertenecer a los dos lenguajes
    for (int i = 0; i < A.cantidad; i++)
    {
        if (existeEnLenguaje(B, A.palabras[i]))
            resultado.palabras[resultado.cantidad++] = A.palabras[i];
    }

    return resultado;
}

// Diferencia de dos lenguajes: resultado = A - B
// Retorna un nuevo lenguaje con las palabras que estan en A pero NO en B
Lenguaje diferenciaCadenas(Lenguaje A, Lenguaje B)
{
    Lenguaje resultado;
    resultado.cantidad = 0;

    // Recorremos A y verificamos si cada palabra NO existe en B
    // Solo se agrega si NO pertenece a B, es decir, es exclusiva de A
    for (int i = 0; i < A.cantidad; i++)
    {
        if (!existeEnLenguaje(B, A.palabras[i]))
            resultado.palabras[resultado.cantidad++] = A.palabras[i];
    }

    return resultado;
}

// Función auxiliar para concatenar dos lenguajes: R = A * B
Lenguaje concatenarLenguajes(Lenguaje A, Lenguaje B)
{
    Lenguaje resultado;
    resultado.cantidad = 0;

    for (int i = 0; i < A.cantidad; i++)
    {
        for (int j = 0; j < B.cantidad; j++)
        {
            // Tratamos "e" como cadena vacía real "" para concatenar correctamente
            const char *s1 = (strcmp(A.palabras[i], CADENA_VACIA) == 0) ? "" : A.palabras[i];
            const char *s2 = (strcmp(B.palabras[j], CADENA_VACIA) == 0) ? "" : B.palabras[j];

            char *nueva = (char *)malloc(strlen(s1) + strlen(s2) + 1);
            sprintf(nueva, "%s%s", s1, s2);

            // Si el resultado es una cadena de longitud 0, representarla como "e"
            if (strlen(nueva) == 0)
            {
                free(nueva);
                nueva = strdup(CADENA_VACIA);
            }

            // Solo agregamos si no existe ya en el conjunto
            if (!existeEnLenguaje(resultado, nueva) && resultado.cantidad < 100)
            {
                resultado.palabras[resultado.cantidad++] = nueva;
            }
            else
            {
                free(nueva);
            }
        }
    }
    return resultado;
}

// Inverso de un lenguaje: retorna un nuevo lenguaje donde cada palabra
// es el reverso de la palabra correspondiente en L
Lenguaje inverso(Lenguaje L)
{
    Lenguaje resultado;
    resultado.cantidad = 0;

    // Recorremos cada palabra del lenguaje L
    for (int i = 0; i < L.cantidad; i++)
    {
        char *inversa = (char *)malloc(strlen(L.palabras[i]) + 1); // Reservamos memoria para la cadena invertida
        int len = strlen(L.palabras[i]);                           // Longitud de la palabra actual
        // Copiamos los caracteres en orden inverso, del ultimo al primero
        for (int j = 0; j < len; j++)
        {
            inversa[j] = L.palabras[i][len - 1 - j]; // Mapeamos posicion j al caracter simetrico
        }
        inversa[len] = '\0'; // Terminamos la cadena con el caracter nulo

        // Agregamos la cadena invertida solo si no existe ya en el resultado
        if (!existeEnLenguaje(resultado, inversa) && resultado.cantidad < 100)
        {
            resultado.palabras[resultado.cantidad++] = inversa; // Insertamos la cadena invertida en el resultado
        }
        else
        {
            free(inversa); // Liberamos la memoria si la cadena ya existe o se alcanzo el limite
        }
    }
    return resultado;
}

// Cerradura opcional de un lenguaje: resultado = L? = L^0 ∪ L^1
// Retorna un nuevo lenguaje que incluye la cadena vacia y todas las palabras de L
Lenguaje cerraduraOpcional(Lenguaje L)
{
    // L? = L^0 U L^1
    Lenguaje vacio;
    vacio.cantidad = 1;
    vacio.palabras[0] = strdup(CADENA_VACIA); // L^0 = {e}

    return unionC(vacio, L); // {e} U L
}

// Cerradura positiva de un lenguaje: resultado = L+ = L^1 ∪ L^2 ∪ ... ∪ L^LIMITE
// Retorna un nuevo lenguaje con todas las concatenaciones de L consigo mismo
// desde la primera potencia hasta el limite definido, sin incluir la cadena vacia
Lenguaje cerraduraPositiva(Lenguaje L)
{
    // Empezamos con L^1 (el propio lenguaje)
    Lenguaje potencia;
    potencia.cantidad = 0;
    for (int i = 0; i < L.cantidad; i++)
        potencia.palabras[potencia.cantidad++] = strdup(L.palabras[i]);

    Lenguaje resultado = potencia; // acumulador, arranca con L^1

    // Iteramos desde L^2 hasta L^LIMITE
    for (int i = 2; i <= LIMITE; i++)
    {
        potencia = concatenarLenguajes(potencia, L); // L^i
        resultado = unionC(resultado, potencia);     // acumula
    }

    // printf("(Resultado truncado en L^%d)\n", LIMITE);
    return resultado;
}

// Cerradura de Kleene de un lenguaje: resultado = L* = L^0 ∪ L^1 ∪ ... ∪ L^LIMITE
// Retorna un nuevo lenguaje que incluye la cadena vacia y todas las
// concatenaciones de L consigo mismo hasta el limite definido
Lenguaje cerraduraKleene(Lenguaje L)
{
    // L* = L^0 U L+
    Lenguaje vacio;
    vacio.cantidad = 1;
    vacio.palabras[0] = strdup(CADENA_VACIA); // L^0 = {e}

    Lenguaje positiva = cerraduraPositiva(L); // L^1 U L^2 U L^3

    // printf("(Resultado truncado en L^%d)\n", LIMITE);
    return unionC(vacio, positiva); // {e} U L+
}

// Imprime en consola todas las palabras de un lenguaje con formato de conjunto
void imprimirLenguaje(Lenguaje L)
{
    printf("{ "); // Apertura del conjunto
    // Recorremos cada palabra del lenguaje e imprimimos separada por comas
    for (int i = 0; i < L.cantidad; i++)
    {
        printf("%s%s", L.palabras[i], (i == L.cantidad - 1) ? "" : ", "); // Omitimos la coma en el ultimo elemento
    }
    printf(" }\n"); // Cierre del conjunto con salto de linea
}

// MENU
// Muestra las opciones y retorna la opcion elegida por el usuario
int menu(void)
{
    int opcion;

    printf("\n======================================\n");
    printf("   Operaciones con Lenguajes\n");
    printf("   Lenguaje L: {1,10,100}\n");
    printf("   Lenguaje M: {10, 01, 11}\n");
    printf("======================================\n");
    printf("  1. Union\n");
    printf("  2. Interseccion\n");
    printf("  3. Diferencia\n");
    printf("  4. Concatenacion\n");
    printf("  5. Inverso\n");
    printf("  6. Potencia\n");
    printf("  7. Cerraduras\n");
    printf("  8. Salir\n");
    printf("--------------------------------------\n");
    printf("  Opcion: ");
    scanf(" %d", &opcion);

    return opcion;
}

int main()
{
    int opcion;

    // Definición de Lenguaje L = {1, 10, 100}
    Lenguaje L;
    L.cantidad = 3;
    L.palabras[0] = "1";
    L.palabras[1] = "10";
    L.palabras[2] = "100";

    // Definición de Lenguaje M = {10, 01, 11}
    Lenguaje M;
    M.cantidad = 3;
    M.palabras[0] = "10";
    M.palabras[1] = "01";
    M.palabras[2] = "11";

    do
    {
        opcion = menu();

        switch (opcion)
        {
        case 1:
        {
            // Imprimimos el resultado entre la union de los lenguajes L y M, utilizando la funcion unionC
            Lenguaje res = unionC(L, M);
            imprimirLenguaje(res);
            break;
        }

        case 2:
        {
            // Imprimimos el resultado entre la interseccion de los lenguajes L y M, utilizando la funcion interseccionCadenas
            Lenguaje res = interseccionCadenas(L, M);
            imprimirLenguaje(res);
            break;
        }

        case 3:
        {
            // Imprimimos el resultado entre la diferencia de los lenguajes L y M, utilizando la funcion diferenciaCadenas
            Lenguaje res = diferenciaCadenas(L, M);
            printf("Diferencia L - M: ");
            imprimirLenguaje(res);
            // IMprimimos el resultado entre la diferencia de los lenguajes M y L, utilizando la funcion diferenciaCadenas
            Lenguaje res2 = diferenciaCadenas(M, L);
            printf("Diferencia M - L: ");
            imprimirLenguaje(res2);
            break;
        }

        case 4:
        {
            // Imprimimos el resultado entre la concatenacion de los lenguajes L y M, utilizando la funcion concatenarLenguajes
            Lenguaje res = concatenarLenguajes(L, M);
            printf("Concatenacion L * M: ");
            imprimirLenguaje(res);
            // Imprimimos el resultado entre la concatenacion de los lenguajes M y L, utilizando la funcion concatenarLenguajes
            Lenguaje res2 = concatenarLenguajes(M, L);
            printf("Concatenacion M * L: ");
            imprimirLenguaje(res2);
            break;
        }

        case 5:
        {
            // Imprimimos el resultado entre el inverso de los lenguaje L
            Lenguaje res = inverso(L);
            printf("Inverso de L: ");
            imprimirLenguaje(res);
            // Imprimimos el resultado entre el inverso de los lenguaje M
            Lenguaje resM = inverso(M);
            printf("Inverso de M: ");
            imprimirLenguaje(resM);
            break;
        }

        case 6:
        {
            int n, sel;
            printf("Selecciona lenguaje (1 = L, 2 = M): ");
            scanf("%d", &sel);
            printf("Exponente n: ");
            scanf("%d", &n);
            Lenguaje base = (sel == 1) ? L : M;
            Lenguaje res;
            res.cantidad = 0;

            // Potencia negativa: L^-n = (L^R)^|n|
            if (n < 0)
            {
                int exp = -n; 

                // L^-n = (L^R)^|n|
                printf("(Potencia negativa: L^%d)\n", n);
                Lenguaje inv = inverso(base);
                // Reemplazamos base con las palabras del inverso usando strdup
                base.cantidad = 0; 
                for (int i = 0; i < inv.cantidad; i++)
                    base.palabras[base.cantidad++] = strdup(inv.palabras[i]);
                // Liberamos el inverso temporal (ya copiamos con strdup)
                for (int i = 0; i < inv.cantidad; i++)
                    free(inv.palabras[i]);
                n = exp; 
            }

            if (n == 0)
            {
                res.cantidad = 1;
                res.palabras[0] = strdup(CADENA_VACIA);
            }
            else
            {
                // Inicializamos res como una copia de la base para n=1
                res.cantidad = 0;
                for (int i = 0; i < base.cantidad; i++)
                    res.palabras[res.cantidad++] = strdup(base.palabras[i]);

                // Multiplicamos n-1 veces
                for (int i = 1; i < n; i++)
                {
                    Lenguaje temp = concatenarLenguajes(res, base);
                    // Liberar memoria del paso anterior
                    for (int j = 0; j < res.cantidad; j++)
                        free(res.palabras[j]);
                    res = temp;
                }
            }

            // Imprimir y Liberar Memoria
            printf("Resultado: { ");
            for (int i = 0; i < res.cantidad; i++)
            {
                printf("%s%s", res.palabras[i], (i == res.cantidad - 1) ? "" : ", ");
                free(res.palabras[i]);
            }
            printf(" }\n");
            break;
        }

        case 7:
        {
            printf("(Resultado truncado en L^%d)\n\n", LIMITE);

            // Cerradura opcional L? y M?
            Lenguaje optL = cerraduraOpcional(L);
            printf("Cerradura opcional L?: ");
            imprimirLenguaje(optL);

            Lenguaje optM = cerraduraOpcional(M);
            printf("Cerradura opcional M?: ");
            imprimirLenguaje(optM);

            // Cerradura positiva L+ y M+
            Lenguaje posL = cerraduraPositiva(L);
            printf("\nCerradura positiva L+: ");
            imprimirLenguaje(posL);

            Lenguaje posM = cerraduraPositiva(M);
            printf("\nCerradura positiva M+: ");
            imprimirLenguaje(posM);

            // Cerradura de Kleene L* y M*
            Lenguaje kleeneL = cerraduraKleene(L);
            printf("\nCerradura de Kleene L*: ");
            imprimirLenguaje(kleeneL);

            Lenguaje kleeneM = cerraduraKleene(M);
            printf("\nCerradura de Kleene M*: ");
            imprimirLenguaje(kleeneM);

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

    return 0;
}