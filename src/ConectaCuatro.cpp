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
        cout << "1 2 3 4 5 6 7" << endl; // Indicador de columnas
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
                if (checkDireccion(fila, col, 0, 1, jugador) || // Horizontal
                    checkDireccion(fila, col, 1, 0, jugador) || // Vertical
                    checkDireccion(fila, col, 1, 1, jugador) || // Diagonal derecha
                    checkDireccion(fila, col, 1, -1, jugador))  // Diagonal izquierda
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
    int evaluarEstado(int jugador) {
        int oponente = (jugador == JUGADOR) ? IA : JUGADOR;
        return contarLineas(jugador, 4) * 1000 - contarLineas(oponente, 4) * 1000 +
               contarLineas(jugador, 3) * 10 - contarLineas(oponente, 3) * 10 +
               contarLineas(jugador, 2) * 1 - contarLineas(oponente, 2) * 1;
    }
    // Nueva función detectarBloqueo
    int detectarBloqueo() {
        for (int fila = 0; fila < filas; ++fila) {
            for (int col = 0; col < columnas; ++col) {
                // Revisar las direcciones en las que la IA tiene 3 fichas consecutivas
                if (checkDireccion(fila, col, 0, 1, IA) || // Horizontal
                    checkDireccion(fila, col, 1, 0, IA) || // Vertical
                    checkDireccion(fila, col, 1, 1, IA) || // Diagonal derecha
                    checkDireccion(fila, col, 1, -1, IA))  // Diagonal izquierda
                {
                    if (esMovimientoValido(col)) {
                        return col; // Retorna la columna en la que bloquear
                    }
                }
            }
        }
        return -1; // Si no hay necesidad de bloquear
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
    int contarLineas(int jugador, int longitud) {
        int total = 0;
        for (int fila = 0; fila < filas; ++fila) {
            for (int col = 0; col < columnas; ++col) {
                if (cuentaLineasDireccion(fila, col, 0, 1, jugador, longitud) ||
                    cuentaLineasDireccion(fila, col, 1, 0, jugador, longitud) ||
                    cuentaLineasDireccion(fila, col, 1, 1, jugador, longitud) ||
                    cuentaLineasDireccion(fila, col, 1, -1, jugador, longitud)) {
                    total++;
                }
            }
        }
        return total;
    }
    bool cuentaLineasDireccion(int fila, int col, int deltaFila, int deltaCol, int jugador, int longitud) {
        int count = 0;
        for (int i = 0; i < longitud; ++i) {
            int nuevaFila = fila + i * deltaFila;
            int nuevaCol = col + i * deltaCol;
            if (nuevaFila >= 0 && nuevaFila < filas && nuevaCol >= 0 && nuevaCol < columnas &&
                (tablero[nuevaFila][nuevaCol] == jugador || tablero[nuevaFila][nuevaCol] == vacío)) {
                ++count;
            } else {
                break;
            }
        }
        return count == longitud;
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
    std::ifstream inputFile("movimientos.txt"); // Archivo de entrada sobre los movimientos del usuario
    if (!inputFile) {
        std::cerr << "Error: No se pudo abrir el archivo de entrada." << std::endl;
        return 1;
    }

    Tablero tablero;
    bool turnoIA = false; // Determina si es el turno de la IA o del jugador
    cout << "========================" << endl;
    cout << "¡Bienvenido a Conecta Cuatro!" << endl;
    cout << "========================" << endl;
    cout << "Instrucciones: " << endl;
    cout << "- Juega seleccionando columnas del 1 al 7." << endl;
    cout << "- El objetivo es conectar 4 fichas consecutivas antes que tu rival." << endl;
    cout << "- Puede ser por arriba, izquierda, derecha o en diagonal." << endl;
    cout << "========================" << endl;

    while (true) {
        tablero.imprimirTablero(); // Mostrar el tablero actualizado

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
            // Primero intentamos bloquear, si es necesario
            int columnaDefensa = tablero.detectarBloqueo();
            if (columnaDefensa != -1) {
                // Si hay un bloqueo necesario, la IA lo hace
                cout << "¡Bloqueando en la columna " << columnaDefensa + 1 << "!" << endl;
                tablero.hacerMovimiento(columnaDefensa, IA);
            } else {
                // Si no hay un bloqueo, la IA elige el mejor movimiento posible
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
            }
        } else {
            // Si no es el turno de la IA, es el turno del jugador
            int columnaDefensa = tablero.detectarBloqueo();
            if (columnaDefensa != -1) {
                // Si hay necesidad de bloquear, lo hace
                cout << "¡Bloqueando en la columna " << columnaDefensa + 1 << "!" << endl;
                tablero.hacerMovimiento(columnaDefensa, JUGADOR);
            } else {
                // Si no es necesario bloquear, el jugador hace un movimiento normal
                int columna;
                
                // Verificar si ya no hay más movimientos en el archivo
                if (inputFile.eof()) {
                    // Si el archivo está vacío, se permite al jugador ingresar un movimiento
                    cout << "No hay más movimientos en el archivo. Ahora, ingresa una columna (1-7): ";
                    cin >> columna;
                } else {
                    // Si aún hay movimientos en el archivo, leer desde el archivo
                    inputFile >> columna;
                }

                // Validar que el archivo tiene datos y la columna es válida
                while (inputFile.fail() || columna < 1 || columna > 7 || !tablero.esMovimientoValido(columna - 1)) {
                    // Si el movimiento no es válido, pedir un nuevo movimiento
                    cout << "Columna inválida o llena. Intenta con otra columna (1-7): ";
                    inputFile.clear();  // Limpiar el estado de error de inputFile
                    inputFile.ignore(numeric_limits<streamsize>::max(), '\n');  // Ignorar cualquier dato no válido
                    cin >> columna;  // Intentar leer una nueva columna
                }

                // Ajustar la columna para que sea 0-6 internamente
                columna -= 1;

                // Hacer el movimiento si es válido
                tablero.hacerMovimiento(columna, JUGADOR);
            }
        }
        turnoIA = !turnoIA; // Cambiar de turno
    }
    cout << "Gracias por jugar Conecta Cuatro. ¡Adiós!" << endl;
    return 0;
}

