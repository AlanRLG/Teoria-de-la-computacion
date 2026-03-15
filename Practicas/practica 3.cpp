/*
Autores: Red One
    Garcia Ambrosio Aldo
    Leaños Gutierrez Alan Rodrigo
    Perez Marquez David Andrawi
Fecha de creacion: 11/03/26
Descripción: Programa en C++ que realiza operaciones sobre cadenas
             binarias sobre el alfabeto {0,1}.
             Incluye validacion de alfabeto y menu interactivo iterativo.
Version: 3.0 (C++)
*/

#include <iostream>
#include <cstring>
#include <string>

using namespace std;

// DEFINICION DEL ALFABETO
// {0, 1}: unico conjunto de simbolos validos para las cadenas
const char ALFABETO_0 = '0';
const char ALFABETO_1 = '1';
const string CADENA_VACIA = "e";

// Verifica que cada caracter de la cadena pertenezca a {0,1}
// Parametro: cadena - cadena a validar
// Retorna true si la cadena es valida, false en caso contrario
bool esValida(const string &cadena)
{
    if (cadena.empty()) return false; // rechaza cadena vacia

    // Recorre cada caracter de la cadena y verifica que sea '0' o '1'
    for (char c : cadena)
    {
        if (c != ALFABETO_0 && c != ALFABETO_1)
            return false; // caracter fuera del alfabeto
    }

    return true;
}

// Cuenta e imprime el numero de caracteres de la cadena
// Parametro: cadena - cadena de la que se calculara la longitud
// La notacion |cadena| representa la longitud en teoria de lenguajes
void calcularLongitud(const string &cadena)
{
    cout << "  |" << cadena << "| = " << cadena.size() << "\n";
}

// Compara dos cadenas caracter a caracter
// Parametros: a, b - cadenas a comparar
// Retorna true si son identicas, false en caso contrario
bool sonIguales(const string &a, const string &b)
{
    return a == b;
}

// Verifica si una cadena es palindromo (se lee igual al derecho y al reves)
// Construye la cadena invertida y la compara con la original
// Parametro: s - cadena a verificar
// Retorna true si cada par de posiciones simetricas coincide, false en caso contrario
bool esPalindromo(const string &s)
{
    int len = s.size();
    string invertida = "";
    int i;

    // Construir la cadena invertida recorriendo de derecha a izquierda
    for (i = len - 1; i >= 0; i--)
    {
        invertida += s[i];
    }

    // Comparar la cadena original con la invertida caracter a caracter
    for (i = 0; i < len; i++)
    {
        if (s[i] != invertida[i])
            return false; // encontramos una diferencia: no es palindromo
    }

    return true;
}

// Busca todas las ocurrencias de 'texto' dentro de 'cadena' y las sustituye por 'textoReemplazo'
// Parametros:
//   cadena        - cadena original sobre la que se realiza el reemplazo (copia local)
//   texto         - subcadena a buscar dentro de cadena
//   textoReemplazo - subcadena con la que se sustituira cada ocurrencia encontrada
// Si no se encuentran ocurrencias, imprime un mensaje indicandolo
void reemplazaCadenas(string cadena, string &texto, string &textoReemplazo)
{
    int pos;
    bool encontrado = false;

    // Busca la subcadena, reemplaza y vuelve a buscar hasta que no haya mas ocurrencias
    // El incremento del for vuelve a buscar desde el inicio despues de cada reemplazo
    for (pos = cadena.find(texto); pos != string::npos; pos = cadena.find(texto))
    {
        // Reemplaza 'texto.size()' caracteres desde 'pos' por 'textoReemplazo'
        cadena.replace(pos, texto.size(), textoReemplazo);
        encontrado = true;
    }

    if (encontrado)
        cout << "Cadena reemplazada: " << cadena << endl;
    else
        cout << "Cadena " << texto << " no encontrada en la cadena " << cadena << endl;
}

// Elimina todos los espacios en blanco y tabuladores de una cadena
// Parametro: s - cadena original con posibles espacios
// Retorna una nueva cadena con solo los caracteres no blancos
string quitarEspacios(const string &s)
{
    int len = s.size();
    string resultado = "";
    int i;

    // Recorre la cadena y agrega al resultado solo los caracteres que no sean espacio ni tabulador
    for (i = 0; i < len; i++)
    {
        if (s[i] != ' ' && s[i] != '\t')
            resultado += s[i]; // solo agrega el caracter si no es espacio
    }

    return resultado;
}

// Calcula el complemento a 1 de una cadena binaria
// Invierte cada bit: '0' se convierte en '1' y '1' se convierte en '0'
// Parametro: s - cadena binaria original
// Retorna la cadena con todos sus bits invertidos
string complementoUno(const string &s)
{
    string resultado = s;

    // Recorre cada caracter y lo invierte usando el operador ternario
    for (char &c : resultado)
        c = (c == '0') ? '1' : '0';

    return resultado;
}

// Busca si 'texto' existe como subcadena dentro de 'cadena'
// Utiliza strstr de cstring que retorna un puntero a la primera ocurrencia
// Parametros:
//   cadena - cadena donde se realizara la busqueda
//   texto  - subcadena a buscar
// Retorna true si fue encontrada, false en caso contrario
bool busquedaCadena(const string &cadena, const string &texto)
{
    // c_str() convierte string a char* que es lo que necesita strstr
    char *resultado = strstr(cadena.c_str(), texto.c_str());

    if (resultado != nullptr)
        return true;  // strstr retorno un puntero valido: la subcadena existe
    else
        return false; // strstr retorno nullptr: la subcadena no existe
}

// MENU
// Muestra las opciones disponibles y retorna la opcion elegida por el usuario
// El menu itera hasta que el usuario elija la opcion de salir
int menu()
{
    int opcion;

    cout << "\n======================================\n";
    cout << "   Operaciones sobre el Alfabeto\n";
    cout << "======================================\n";
    cout << "  1. Longitud\n";
    cout << "  2. Igualdad\n";
    cout << "  3. Palindromo\n";
    cout << "  4. Reemplazo de cadenas\n";
    cout << "  5. Quitar espacios en blanco\n";
    cout << "  6. Complemento a 1\n";
    cout << "  7. Busqueda de una cadena dentro de otra\n";
    cout << "  8. Salir\n";
    cout << "--------------------------------------\n";
    cout << "  Opcion: ";
    cin >> opcion;

    return opcion;
}

int main()
{
    string cadena1, cadena2, texto, textoReemplazo;
    int opcion;

    cout << "= Cadenas y Lenguajes =\n";
    cout << "  Alfabeto: Sigma = {0, 1}\n\n";

    // Leer cadena 1 con validacion de alfabeto
    // Repite hasta recibir una cadena valida sobre {0,1}
    do {
        cout << "Ingrese la cadena 1 (solo '0' y '1'): ";
        cin >> texto;
        if (!esValida(texto))
            cout << "  ERROR: La cadena contiene simbolos fuera de {0,1}. Intente de nuevo.\n";
    } while (!esValida(texto));

    cadena1 = texto;

    // Leer cadena 2 con validacion de alfabeto
    // Repite hasta recibir una cadena valida sobre {0,1}
    do {
        cout << "Ingrese la cadena 2 (solo '0' y '1'): ";
        cin >> texto;
        if (!esValida(texto))
            cout << "  ERROR: La cadena contiene simbolos fuera de {0,1}. Intente de nuevo.\n";
    } while (!esValida(texto));

    cadena2 = texto;

    // Ciclo principal del menu: itera hasta que el usuario elija la opcion 8 (Salir)
    do {
        opcion = menu();

        switch (opcion)
        {
            case 1:
            {
                // Calcula e imprime la longitud de ambas cadenas
                cout << "\n-- Longitud --\n";
                calcularLongitud(cadena1);
                calcularLongitud(cadena2);
                break;
            }

            case 2:
            {
                // Compara las dos cadenas e indica si son iguales o distintas
                cout << "\n-- Igualdad --\n";
                if (sonIguales(cadena1, cadena2))
                    cout << "  \"" << cadena1 << "\" == \"" << cadena2 << "\": Son IGUALES\n";
                else
                    cout << "  \"" << cadena1 << "\" != \"" << cadena2 << "\": Son DISTINTAS\n";
                break;
            }

            case 3:
            {
                // Verifica si cada cadena es palindromo e imprime el resultado
                cout << "\n-- Palindromo --\n";
                cout << "  \"" << cadena1 << "\": "
                     << (esPalindromo(cadena1) ? "ES palindromo" : "NO es palindromo") << "\n";
                cout << "  \"" << cadena2 << "\": "
                     << (esPalindromo(cadena2) ? "ES palindromo" : "NO es palindromo") << "\n";
                break;
            }

            case 4:
            {
                // Solicita la subcadena a buscar y la subcadena de reemplazo
                // Valida que el reemplazo no sea mas largo que la cadena buscada
                // Aplica el reemplazo sobre ambas cadenas
                cout << "\n-- Reemplazo de cadenas --\n";
                cout << "Ingrese la cadena a reemplazar: ";
                cin.ignore(); // limpia el salto de linea pendiente en el buffer
                getline(cin, texto);
                cout << "Ingrese la cadena de reemplazo: ";
                getline(cin, textoReemplazo);

                if (textoReemplazo.size() > texto.size())
                {
                    cout << "La cadena de reemplazo es mas grande que la cadena a reemplazar" << endl;
                    break;
                }
                else
                {
                    cout << "Cadena 1: ";
                    reemplazaCadenas(cadena1, texto, textoReemplazo);
                    cout << "Cadena 2: ";
                    reemplazaCadenas(cadena2, texto, textoReemplazo);
                }
                break;
            }

            case 5:
            {
                // Las cadenas binarias no contienen espacios por definicion,
                // por lo que esta operacion se demuestra sobre una cadena libre ingresada por el usuario
                string textoLibre;
                cout << "\n-- Quitar espacios en blanco --\n";
                cout << "  Ingrese una cadena con posibles espacios: ";
                cin.ignore(); // limpia el salto de linea pendiente en el buffer
                getline(cin, textoLibre);
                cout << "  Original : \"" << textoLibre << "\"\n";
                cout << "  Sin espacios: \"" << quitarEspacios(textoLibre) << "\"\n";
                break;
            }

            case 6:
            {
                // Calcula e imprime el complemento a 1 de ambas cadenas
                cout << "\n-- Complemento a Uno --\n";
                cout << "  complemento(" << cadena1 << ") = " << complementoUno(cadena1) << "\n";
                cout << "  Complemento(" << cadena2 << ") = " << complementoUno(cadena2) << "\n";
                break;
            }

            case 7:
            {
                // Solicita una subcadena y la busca dentro de cadena1 y cadena2
                // Imprime si fue encontrada o no en cada caso
                cout << "\n-- Busqueda de una cadena dentro de otra --\n";
                cout << "Ingrese la cadena a buscar: ";
                cin.ignore(); // limpia el salto de linea pendiente en el buffer
                getline(cin, texto);

                // Busqueda en cadena1
                if (busquedaCadena(cadena1, texto))
                    cout << "Cadena " << texto << " encontrada en la cadena " << cadena1 << endl;
                else
                    cout << "Cadena " << texto << " no encontrada en la cadena " << cadena1 << endl;

                // Busqueda en cadena2
                if (busquedaCadena(cadena2, texto))
                    cout << "Cadena " << texto << " encontrada en la cadena " << cadena2 << endl;
                else
                    cout << "Cadena " << texto << " no encontrada en la cadena " << cadena2 << endl;
                break;
            }

            case 8:
            {
                // Termina el ciclo do-while y finaliza el programa
                cout << "\nSaliendo del programa.\n";
                break;
            }

            default:
                cout << "Opcion invalida. Intente de nuevo.\n";
        }

    } while (opcion != 8);

    return 0;
}