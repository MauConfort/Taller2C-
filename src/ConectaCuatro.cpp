/*
 * ConectaCuatro.cpp
 *
 *  Created on: 24-11-2024
 *      Author: Mauricio Muñoz
 *      RUT: 21.542.213-5
 *      Ramo: Estructura de Datos
 */

#include <iostream>
#include <vector>
#include <limits>
#include <fstream>

using namespace std;

/**
 * @brief Dimensiones del tablero original de Conecta Cuatro.
 */
const int filas = 6; /**< Número de filas del tablero. */
const int columnas = 7; /**< Número de columnas del tablero. */
const int vacío = 0; /**< Valor que representa una celda vacía. */
const int JUGADOR = 1; /**< Valor que representa una celda ocupada por el jugador. */
const int IA = 2; /**< Valor que representa una celda ocupada por la IA. */
const int profundidad_max = 7; /**< Profundidad máxima o dificultad del algoritmo Minimax. */

/**
 * @class Tablero
 * @brief Representa el tablero del juego Conecta Cuatro.
 */
class Tablero {
    vector<vector<int>> tablero; /**< Matriz que representa el estado actual del tablero. */
public:
    /**
     * @brief Constructor de la clase Tablero.
     * Inicializa el tablero con celdas vacías.
     */
    Tablero() : tablero(filas, vector<int>(columnas, vacío)) {}

    /**
     * @brief Imprime el estado actual del tablero en consola.
     */
    void imprimirTablero() {
        for (const auto& fila : tablero) {
            for (int celda : fila) {
                cout << (celda == vacío ? "." : (celda == JUGADOR ? "O" : "X")) << " ";
            }
            cout << endl;
        }
        cout << "1 2 3 4 5 6 7" << endl;
    }

    /**
     * @brief Verifica si un movimiento en la columna dada es válido.
     * @param columna Índice de la columna (0 a 6).
     * @return `true` si el movimiento es válido; de lo contrario, `false`.
     */
    bool esMovimientoValido(int columna) {
        return columna >= 0 && columna < columnas && tablero[0][columna] == vacío;
    }

    /**
     * @brief Realiza un movimiento colocando una ficha en la columna dada.
     * @param columna Índice de la columna (0 a 6).
     * @param jugador Identificador del jugador (JUGADOR o IA).
     * @return `true` si el movimiento se realizó correctamente; de lo contrario, `false`.
     */
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

    /**
     * @brief Revierte el último movimiento en la columna dada.
     * @param columna Índice de la columna (0 a 6).
     */
    void deshacerMovimiento(int columna) {
        for (int fila = 0; fila < filas; ++fila) {
            if (tablero[fila][columna] != vacío) {
                tablero[fila][columna] = vacío;
                break;
            }
        }
    }

    /**
     * @brief Verifica si el jugador dado ha ganado el juego.
     * @param jugador Identificador del jugador (JUGADOR o IA).
     * @return `true` si el jugador ha ganado; de lo contrario, `false`.
     */
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

    /**
     * @brief Verifica si el tablero está lleno.
     * @return `true` si el tablero está lleno; de lo contrario, `false`.
     */
    bool estaLleno() {
        for (int col = 0; col < columnas; ++col) {
            if (esMovimientoValido(col)) return false;
        }
        return true;
    }

    /**
     * @brief Evalúa el estado actual del tablero para un jugador.
     * @param jugador Identificador del jugador (JUGADOR o IA).
     * @return Valor numérico que representa la evaluación del tablero.
     */
    int evaluarEstado(int jugador) {
        int oponente = (jugador == JUGADOR) ? IA : JUGADOR;
        return contarLineas(jugador, 4) * 1000 - contarLineas(oponente, 4) * 1000 +
               contarLineas(jugador, 3) * 10 - contarLineas(oponente, 3) * 10 +
               contarLineas(jugador, 2) * 1 - contarLineas(oponente, 2) * 1;
    }

    /**
     * @brief Detecta si la IA necesita bloquear al jugador en una columna específica.
     * @return Índice de la columna para bloquear, o `-1` si no se requiere bloqueo.
     */
    int detectarBloqueo() {
        for (int fila = 0; fila < filas; ++fila) {
            for (int col = 0; col < columnas; ++col) {
                if (checkDireccion(fila, col, 0, 1, IA) ||
                    checkDireccion(fila, col, 1, 0, IA) ||
                    checkDireccion(fila, col, 1, 1, IA) ||
                    checkDireccion(fila, col, 1, -1, IA)) {
                    if (esMovimientoValido(col)) {
                        return col;
                    }
                }
            }
        }
        return -1;
    }

private:
    /**
     * @brief Verifica si hay una línea de longitud dada en una dirección específica.
     * @param fila Fila inicial.
     * @param col Columna inicial.
     * @param deltaFila Dirección en el eje de las filas.
     * @param deltaCol Dirección en el eje de las columnas.
     * @param jugador Identificador del jugador (JUGADOR o IA).
     * @return `true` si hay una línea del jugador; de lo contrario, `false`.
     */
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

    /**
     * @brief Cuenta las líneas potenciales de una longitud específica que puede formar un jugador.
     * @param jugador Identificador del jugador (JUGADOR o IA).
     * @param longitud Número de fichas consecutivas que se buscan.
     * @return Número total de líneas potenciales encontradas para el jugador.
     */
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

    /**
     * @brief Verifica si existe una línea continua o potencial para un jugador en una dirección específica.
     * @param fila Índice de la fila de inicio.
     * @param col Índice de la columna de inicio.
     * @param deltaFila Incremento para moverse en filas (dirección vertical).
     * @param deltaCol Incremento para moverse en columnas (dirección horizontal).
     * @param jugador Identificador del jugador (JUGADOR o IA).
     * @param longitud Número de fichas consecutivas que se buscan.
     * @return `true` si se encuentra una línea de la longitud buscada; de lo contrario, `false`.
     */
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

/**
 * @brief Implementa el algoritmo Minimax con poda alpha-beta para evaluar y seleccionar el mejor movimiento.
 * @param tablero Referencia al objeto Tablero que representa el estado actual del juego.
 * @param profundidad Número máximo de niveles que el algoritmo explorará.
 * @param esIA `true` si es el turno de la IA; `false` si es el turno del jugador.
 * @param alpha Mejor valor que el jugador maximizador puede asegurar (poda alpha).
 * @param beta Mejor valor que el jugador minimizador puede asegurar (poda beta).
 * @param jugador Identificador del jugador actual (JUGADOR o IA).
 * @return Evaluación numérica del mejor movimiento disponible.
 */
int minimax(Tablero& tablero, int profundidad, bool esIA, int alpha, int beta, int jugador) {
    if (tablero.hayGanador(JUGADOR)) return -1000 + profundidad;
    if (tablero.hayGanador(IA)) return 1000 - profundidad;
    if (tablero.estaLleno() || profundidad == 0) return 0;

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
    Tablero tablero;
    bool turnoIA = false;
    int columna;

    cout << "========================" << endl;
    cout << "¡Bienvenido a Conecta Cuatro!" << endl;
    cout << "========================" << endl;
    cout << "Instrucciones: " << endl;
    cout << "- Juega seleccionando columnas del 1 al 7." << endl;
    cout << "- El objetivo es conectar 4 fichas consecutivas antes que tu rival." << endl;
    cout << "- Puede ser por arriba, izquierda, derecha, abajo o en diagonal." << endl;
    cout << "========================" << endl;

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

            int columnaDefensa = tablero.detectarBloqueo();
            if (columnaDefensa != -1) {
                cout << "¡Bloqueando en la columna " << columnaDefensa + 1 << "!" << endl;
                tablero.hacerMovimiento(columnaDefensa, IA);
            } else {
                for (int col = 0; col < columnas; ++col) {
                    if (tablero.esMovimientoValido(col)) {
                        tablero.hacerMovimiento(col, IA);
                        int eval = minimax(tablero, profundidad_max, false, numeric_limits<int>::min(), numeric_limits<int>::max(), IA);
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
            cout << "¡Es tu turno!" << endl;
            int columnaDefensa = tablero.detectarBloqueo();
            if (columnaDefensa != -1) {
                cout << "¡Bloqueando en la columna " << columnaDefensa + 1 << "!" << endl;
                tablero.hacerMovimiento(columnaDefensa, JUGADOR);
            } else {
                // Usando cin para que el jugador elija una columna
                do {
                    cout << "Selecciona una columna (1-7): ";
                    cin >> columna;
                    columna--;  // Convertir columna a índice 0-6
                } while (columna < 0 || columna >= 7 || !tablero.esMovimientoValido(columna));

                tablero.hacerMovimiento(columna, JUGADOR);
            }
        }

        turnoIA = !turnoIA;
    }
    cout << "Gracias por jugar Conecta Cuatro. ¡Adiós!" << endl;
}

