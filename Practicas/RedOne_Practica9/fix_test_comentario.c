// Prueba del BUG 1: comentario de bloque que nunca cierra.
int main() {
    int x = 10;
    /* A partir de aqui empieza un comentario de bloque
       que se extiende durante varias lineas
       y se "olvida" de cerrarse correctamente,
       llegando hasta el final del archivo.
    int y = 5;
    return 0;
}
