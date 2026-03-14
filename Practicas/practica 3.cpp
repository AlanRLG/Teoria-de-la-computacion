/*
Autores: Red One
    Garcia Ambrosio Aldo
    Leaños Gutierrez Alan Rodrigo
    Perez Marquez David Andrawi
Fecha de creacion: 11/03/26
Descripción:

Version: 3.0 (C++)
*/

#include <iostream>
#include <string>

using namespace std;

// DEFINICION DEL ALFABETO
// {0, 1}: unico conjunto de simbolos validos para las cadenas
const char ALFABETO_0 = '0';
const char ALFABETO_1 = '1';
const string CADENA_VACIA = "e";

// Verifica que cada caracter de la cadena pertenezca a {0,1}
// Retorna true si la cadena es valida, false en caso contrario
bool esValida(const string &cadena)
{
    if (cadena.empty()) return false; // rechaza cadena vacia

    for (char c : cadena)
    {
        if (c != ALFABETO_0 && c != ALFABETO_1)
            return false; // caracter fuera del alfabeto
    }

    return true;
}

// Cuenta e imprime el numero de caracteres de la cadena
void calcularLongitud(const string &cadena)
{
    cout << "  |" << cadena << "| = " << cadena.size() << "\n";
}

string complementoUno(const string &s)
{
    string resultado = s;

    for (char &c : resultado)
        c = (c == '0') ? '1' : '0';

    return resultado;
}

// Compara dos cadenas caracter a caracter
// Retorna true si son identicas, false en caso contrario
bool sonIguales(const string &a, const string &b)
{
    return a == b;
}

// Verifica si una cadena es palindromo (se lee igual al derecho y al reves)
// Usa dos indices: izq avanza de izquierda a derecha, der retrocede de derecha a izquierda
// Retorna true si cada par de posiciones simetricas coincide, false en caso contrario
bool esPalindromo(const string &s)
{
    int len = s.size();
    string invertida = "";
    int i;
 
    // construir la cadena invertida recorriendo de derecha a izquierda
    for (i = len - 1; i >= 0; i--)
    {
        invertida += s[i];
    }
 
    // comparar la cadena original con la invertida, de izquierda a derecha
    for (i = 0; i < len; i++)
    {
        if (s[i] != invertida[i])
            return false; // encontramos una diferencia: no es palindromo
    }
 
    return true;
}

// Elimina todos los espacios en blanco de una cadena
// Retorna la cadena sin espacios
string quitarEspacios(const string &s)
{
    int len = s.size();
    string resultado = "";
    int i;
 
    for (i = 0; i < len; i++)
    {
        if (s[i] != ' ' && s[i] != '\t')
            resultado += s[i]; // solo agrega el caracter si no es espacio
    }
 
    return resultado;
}

// MENU

// Muestra las opciones y retorna la opcion elegida por el usuario
int menu()
{
    int opcion;

    cout << "\n======================================\n";
    cout << "   Operaciones sobre el Alfabeto\n";
    cout << "======================================\n";
    cout << "  1. Longitud\n";
    cout << "  2. Igualdad\n";
    cout << "  3. Palíndromo\n";
    cout << "  4. Reemplazo de cadenas\n";
    cout << "  5. Quitar espacios en blanco\n";
    cout << "  6. Complemento a 1\n";
    cout << "  7. Búsqueda de una cadena dentro de otra\n";
    cout << "  8. Salir\n";
    cout << "--------------------------------------\n";
    cout << "  Opcion: ";
    cin >> opcion;

    return opcion;
}

int main()
{
    string cadena1, cadena2, texto;
    int opcion;

    cout << "= Cadenas y Lenguajes =\n";
    cout << "  Alfabeto: Sigma = {0, 1}\n\n";

    // Leer cadena 1 con validacion de alfabeto; repite hasta recibir entrada valida
    do {
        cout << "Ingrese la cadena 1 (solo '0' y '1'): ";
        cin >> texto;
        if (!esValida(texto))
            cout << "  ERROR: La cadena contiene simbolos fuera de {0,1}. Intente de nuevo.\n";
    } while (!esValida(texto));

    cadena1 = texto;

    // Leer cadena 2 con validacion de alfabeto; repite hasta recibir entrada valida
    do {
        cout << "Ingrese la cadena 2 (solo '0' y '1'): ";
        cin >> texto;
        if (!esValida(texto))
            cout << "  ERROR: La cadena contiene simbolos fuera de {0,1}. Intente de nuevo.\n";
    } while (!esValida(texto));

    cadena2 = texto;

    do {
        opcion = menu();

        switch (opcion)
        {
            case 1:
            {
                cout << "\n-- Longitud --\n";
                calcularLongitud(cadena1);
                calcularLongitud(cadena2);
                break;
            }

            case 2:
            {
                cout << "\n-- Igualdad --\n";
                if (sonIguales(cadena1, cadena2))
                    cout << "  \"" << cadena1 << "\" == \"" << cadena2 << "\": Son IGUALES\n";
                else
                    cout << "  \"" << cadena1 << "\" != \"" << cadena2 << "\": Son DISTINTAS\n";
                break;
            }

            case 3:
            {
                cout << "\n-- Palindromo --\n";
                cout << "  \"" << cadena1 << "\": "
                     << (esPalindromo(cadena1) ? "ES palindromo" : "NO es palindromo") << "\n";
                cout << "  \"" << cadena2 << "\": "
                     << (esPalindromo(cadena2) ? "ES palindromo" : "NO es palindromo") << "\n";
                break;
            }

            case 4:
            {

                break;
            }

            case 5:
            {
                // Las cadenas binarias ya no contienen espacios,
                // por lo que esta operacion se demuestra sobre una cadena libre ingresada por el usuario.
                string textoLibre;
                cout << "\n-- Quitar espacios en blanco --\n";
                cout << "  Ingrese una cadena con posibles espacios: ";
                cin.ignore();
                getline(cin, textoLibre);
                cout << "  Original : \"" << textoLibre << "\"\n";
                cout << "  Sin espacios: \"" << quitarEspacios(textoLibre) << "\"\n";
                break;
            }

            case 6:
            {
                cout << "\n-- Complemento a Uno --\n";
                cout << "  complemento(" << cadena1 << ") = " << complementoUno(cadena1) << "\n";
                cout << "  Complemento(" << cadena2 << ") = " << complementoUno(cadena2) << "\n";
                break;
            }

            case 7:
            {

                break;
            }

            case 8:
            {
                cout << "\nSaliendo del programa.\n";
                break;
            }

            default:
                cout << "Opcion invalida. Intente de nuevo.\n";
        }

    } while (opcion != 8);

    return 0;
}