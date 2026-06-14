// Archivo de prueba CORRECTO. No debe reportar errores lexicos ni sintacticos.
int main() {
    int    a = 42;
    float  b = 3.14;
    double c = 0.5;
    char   d = 'z';

    int arr[3];
    arr[0] = a;

    if (a > 0) {
        b = b + c;
    }

    while (a < 100) {
        a = a + 1;
    }

    return 0;
}
