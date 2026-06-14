// Prueba del BUG 2: numeros malformados.
int main() {
    int   a = 42;       // OK
    float b = 3.14;     // OK
    float c = 0.5;      // OK
    float d = 3.;       // ERROR LEXICO: punto sin digitos decimales
    int   e = 123abc;   // ERROR LEXICO: letras pegadas a la cifra
    return 0;
}
