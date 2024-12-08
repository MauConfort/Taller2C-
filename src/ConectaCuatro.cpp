/*
 * ConectaCuatro.cpp
 *
 *  Created on: 24-11-2024
 *      Author: Mauricio Muñoz
 *      RUT: 21.542.213-5
 *      Ramo: Estructura de Datos
 */

#include <iostream>
//Incluir vectores para el tablero
#include <vector>
// Para el minimax :3
#include <limits>
// Metodo para reemplazar el cin
#include <fstream>

using namespace std;

const int filas = 6;
const int columnas = 7;
const int vacío = 0;
const int JUGADOR = 1;
const int IA = 2;
const int profundidad_max = 7;

class Tablero {
    vector<vector<int>> tablero;
public:
    Tablero() : tablero(filas, vector<int>(columnas, vacío)) {}
    void imprimirTablero() {
        for (const auto& fila : tablero) {
            for (int celda : fila) {
                cout << (celda == vacío ? "." : (celda == JUGADOR ? "O" : "X")) << " ";
            }
            cout << endl;
        }
        cout << "1 2 3 4 5 6 7" << endl;
    }
    bool esMovimientoValido(int columna) {
        return columna >= 0 && columna < columnas && tablero[0][columna] == vacío;
    }
    bool hacerMovimiento(int columna, int jugador) {
        if (!esMovimientoValido(columna)) return false;
        for (int fila = filas - 1; fila >= 0; --fila) {
            if (tablero[fila][columna] == vacío) {
                tablero[fila][columna] = jugador;
                return true;
            }
        }
        return false;
    }
    void deshacerMovimiento(int columna) {
        for (int fila = 0; fila < filas; ++fila) {
            if (tablero[fila][columna] != vacío) {
                tablero[fila][columna] = vacío;
                break;
            }
        }
    }
    bool hayGanador(int jugador) {
        for (int fila = 0; fila < filas; ++fila) {
            for (int col = 0; col < columnas; ++col) {
                if (checkDireccion(fila, col, 0, 1, jugador) || 
                    checkDireccion(fila, col, 1, 0, jugador) || 
                    checkDireccion(fila, col, 1, 1, jugador) || 
                    checkDireccion(fila, col, 1, -1, jugador)) 
                    return true;
            }
        }
        return false;
    }
    bool estaLleno() {
        for (int col = 0; col < columnas; ++col) {
            if (esMovimientoValido(col)) return false;
        }
        return true;
    }
private:
    bool checkDireccion(int fila, int col, int deltaFila, int deltaCol, int jugador) {
        int count = 0;
        for (int i = 0; i < 4; ++i) {
            int nuevaFila = fila + i * deltaFila;
            int nuevaCol = col + i * deltaCol;
            if (nuevaFila >= 0 && nuevaFila < filas && nuevaCol >= 0 && nuevaCol < columnas &&
                tablero[nuevaFila][nuevaCol] == jugador) {
                ++count;
            } else {
                break;
            }
        }
        return count == 4;
    }
};

int minimax(Tablero& tablero, int profundidad, bool esIA, int alpha, int beta, int jugador) {
    if (tablero.hayGanador(JUGADOR)) return -1000 + profundidad;
    if (tablero.hayGanador(IA)) return 1000 - profundidad;
    if (tablero.estaLleno() || profundidad == 0) return 0;  // Evaluación simple, podría ser más compleja
    if (esIA) {
        int maxEval = numeric_limits<int>::min();
        for (int col = 0; col < columnas; ++col) {
            if (tablero.esMovimientoValido(col)) {
                tablero.hacerMovimiento(col, IA);
                int eval = minimax(tablero, profundidad - 1, false, alpha, beta, jugador);
                tablero.deshacerMovimiento(col);
                maxEval = max(maxEval, eval);
                alpha = max(alpha, eval);
                if (beta <= alpha) break;
            }
        }
        return maxEval;
    } else {
        int minEval = numeric_limits<int>::max();
        for (int col = 0; col < columnas; ++col) {
            if (tablero.esMovimientoValido(col)) {
                tablero.hacerMovimiento(col, JUGADOR);
                int eval = minimax(tablero, profundidad - 1, true, alpha, beta, jugador);
                tablero.deshacerMovimiento(col);
                minEval = min(minEval, eval);
                beta = min(beta, eval);
                if (beta <= alpha) break;
            }
        }
        return minEval;
    }
}

int main() {
    ifstream inputFile("movimientos.txt");
    if (!inputFile) {
        cerr << "Error: No se pudo abrir el archivo de entrada." << endl;
        return 1;
    }

    Tablero tablero;
    bool turnoIA = false;
    int jugadorMovimiento;

    cout << "¡Bienvenido a Conecta Cuatro!" << endl;

    while (true) {
        tablero.imprimirTablero();

        if (tablero.hayGanador(JUGADOR)) {
            cout << "¡Felicidades! ¡Ganaste!" << endl;
            break;
        }
        if (tablero.hayGanador(IA)) {
            cout << "La IA gana. ¡Suerte para la próxima!" << endl;
            break;
        }
        if (tablero.estaLleno()) {
            cout << "¡Empate! Nadie gana." << endl;
            break;
        }

        if (turnoIA) {
            cout << "Turno de la IA..." << endl;
            int mejorMovimiento = -1;
            int mejorEval = numeric_limits<int>::min();
            for (int col = 0; col < columnas; ++col) {
                if (tablero.esMovimientoValido(col)) {
                    tablero.hacerMovimiento(col, IA);
                    int eval = minimax(tablero, profundidad_max, false, numeric_limits<int>::min(),
                                       numeric_limits<int>::max(), IA);
                    tablero.deshacerMovimiento(col);
                    if (eval > mejorEval) {
                        mejorEval = eval;
                        mejorMovimiento = col;
                    }
                }
            }
            tablero.hacerMovimiento(mejorMovimiento, IA);
        } else {
            cout << "Tu turno. Elige una columna (1-7): ";
            if (!(inputFile >> jugadorMovimiento) || jugadorMovimiento < 1 || jugadorMovimiento > 7) {
                cout << "Movimiento inválido o archivo vacío. Terminado." << endl;
                break;
            }

            if (!tablero.hacerMovimiento(jugadorMovimiento - 1, JUGADOR)) {
                cout << "Columna llena. Intenta otra columna." << endl;
            }
        }

        turnoIA = !turnoIA;
    }

    inputFile.close();
    cout << "Gracias por jugar Conecta Cuatro. ¡Adiós!" << endl;
    return 0;
}


