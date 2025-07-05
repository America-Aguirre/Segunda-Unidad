#include <iostream>
#include <fstream>
#include <vector>
#include <conio.h>
#include <windows.h>

using namespace std;

// Direcciones para moverse (derecha, abajo, izquierda, arriba)
const int MOV[4][2] = {{1,0}, {0,1}, {-1,0}, {0,-1}};

// Caracteres del laberinto
const char INI = 'E';      // Punto de inicio
const char FIN = 'S';      // Salida
const char PARED1 = '#';   // Pared
const char PARED2 = '*';   // Pared alternativa
const char LIBRE = ' ';    // Espacio vacío
const char RUTA = '.';     // Camino solución

// Estructura para guardar posiciones
struct Punto {
    int x, y; // x = columna, y = fila
    Punto(int px = 0, int py = 0) { x = px; y = py; }
};

// Clase principal para manejar el laberinto
class Laberinto {
    vector<vector<char> > mapa;  // Matriz del laberinto
    Punto entrada;              // Posición de inicio
    bool entradaOk;             // True si encontró la entrada
    int filas, cols;            // Tamaño del laberinto
    
    // Verifica si un caracter es pared
    bool esPared(char c) { return c == PARED1 || c == PARED2; }
    
    // Limpia la pantalla
    void limpiarPantalla() { system("cls"); }
    
public:
    // Constructor
    Laberinto() : entradaOk(false), filas(0), cols(0) {}
    
    // Carga el laberinto desde archivo
    bool cargar(const string& archivo) {
        ifstream f(archivo.c_str());
        if (!f) {
            cout << "Error: No se pudo abrir el archivo" << endl;
            return false;
        }

        string linea;
        int fila = 0;
        
        while (getline(f, linea)) {
            vector<char> filaMapa;
            for (int col = 0; col < (int)linea.size(); col++) {
                filaMapa.push_back(linea[col]);
                if (linea[col] == INI) {
                    entrada = Punto(col, fila);
                    entradaOk = true;
                }
            }
            mapa.push_back(filaMapa);
            fila++;
        }
        
        filas = mapa.size();
        if (filas > 0) cols = mapa[0].size();
        
        if (!entradaOk) {
            cout << "Error: No se encontro el punto de inicio" << endl;
            return false;
        }
        
        return true;
    }

    // Muestra el laberinto en pantalla
    void mostrar() {
        limpiarPantalla();
        for (int i = 0; i < filas; i++) {
            for (int j = 0; j < cols; j++) {
                cout << mapa[i][j];
            }
            cout << endl;
        }
    }

    // Busca la solución al laberinto
    bool resolver() {
        bool resultado = buscarRuta(entrada.x, entrada.y);
        mostrar();
        
        if (resultado) {
            cout << "\nLABERINTO COMPLETADO" << endl;
        } else {
            cout << "\nNO HAY RUTA POSIBLE" << endl;
        }
        
        return resultado;
    }

private:
    // Función recursiva que busca la ruta
    bool buscarRuta(int x, int y) {
        if (mapa[y][x] == FIN) return true; // Llegó a la salida
        if (esPared(mapa[y][x]) || mapa[y][x] == RUTA) return false; // No puede pasar

        char original = mapa[y][x];
        if (original != INI) {
            mapa[y][x] = RUTA; // Marca el camino
        }

        // Prueba las 4 direcciones
        for (int i = 0; i < 4; i++) {
            int nx = x + MOV[i][0];
            int ny = y + MOV[i][1];
            
            if (ny >= 0 && ny < filas && nx >= 0 && nx < cols) {
                if (buscarRuta(nx, ny)) {
                    return true;
                }
            }
        }

        // Si no encontró solución, borra el camino
        if (original != INI) {
            mapa[y][x] = original;
        }
        
        return false;
    }
};

// Función principal
int main() {
    Laberinto lab;
    
    // Carga el laberinto
    if (!lab.cargar("texto.txt")) {
        cout << "Presiona una tecla para salir...";
        _getch();
        return 1;
    }

    // Intenta resolverlo
    lab.resolver();
    
    cout << "Presiona una tecla para salir...";
    _getch();
    return 0;
}
