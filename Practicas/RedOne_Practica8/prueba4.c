// prueba4_switch_sin_case.c
// Prueba 4 - switch cuyo cuerpo no contiene case ni default
// Error esperado: 'switch' en la linea N no tiene 'case' ni 'default'

int procesar(int codigo) {

    int resultado = 0;

    /* switch valido para referencia */
    switch (codigo) {
        case 1:
            resultado = 100;
            break;
        default:
            resultado = -1;
            break;
    }

    /* switch sin ninguna etiqueta case ni default */
    switch (resultado) {
        {
            resultado = resultado * 2;
        }
    }

    return resultado;
}

int main(void) {
    return procesar(1);
}