#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <array>
#include <cctype>

using namespace std;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cerr << "Uso: " << argv[0] << " <archivo.txt>\n";
        return 1;
    }

    ifstream archivo(argv[1]);
    if (!archivo.is_open()) {
        cerr << "Error: No se pudo abrir el archivo '" << argv[1] << "'\n";
        return 1;
    }

    array<long, 26> conteo{};
    long total = 0;
    char c;

    while (archivo.get(c)) {
        if (isalpha(static_cast<unsigned char>(c))) {
            conteo[tolower(static_cast<unsigned char>(c)) - 'a']++;
            total++;
        }
    }
    archivo.close();

    cout << "  Letra  │  Cantidad  │   Porcentaje      \n";

    for (int i = 0; i < 26; i++) {
        double porcentaje = (total > 0) ? (conteo[i] * 100.0 / total) : 0.0;
        cout << "    " << static_cast<char>('A' + i) << "    │  "
                  << setw(7) << conteo[i]
                  << "   │   "
                  << fixed << setprecision(2)
                  << setw(6) << porcentaje << "%         \n";
    }

    cout << "  TOTAL  │  " << setw(7) << total
              << "   │   100.00%         \n";

    return 0;
}