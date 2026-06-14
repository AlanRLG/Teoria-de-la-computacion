// Prueba del BUG 3: error dentro del arreglo no debe disparar errores fantasma.
int main() {
    int x[ ;        // UN unico error real (falta tamano y ']')
    return 0;
}
