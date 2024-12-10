# Conecta Cuatro - Juego con IA
Nombre: Mauricio Javier Muñoz Guerrero;
RUT: 21.542.213-5;
Correo: mauricio.munoz01@alumnos.ucn.cl;
Paralelo: C2;
Fecha: 09/12/2024;

Este es un proyecto de **Conecta Cuatro**, implementado en **C++**. El juego permite que el jugador compita contra una **IA** utilizando el algoritmo **Minimax con poda alpha-beta** para seleccionar los mejores movimientos de la IA. El juego seleccionado es **Conecta Cuatro** en su formato original.

## Tabla de contenidos
- [Descripción](#descripción)
- [Cómo jugar](#cómo-jugar)
- [Características](#características)
- [Requisitos](#requisitos)
- [Instrucciones para ejecutar el programa](#instrucciones-para-ejecutar-el-programa)
- [Algoritmo Minimax](#algoritmo-minimax)
- [Poda Alfa Beta](#poda-alfa-beta)
- [Explicación del juego](#explicación-del-juego)
- [Diagrama del árbol de juego para un caso simple](#diagrama-del-árbol-de-juego-para-un-caso-simple)
- [Análisis de complejidad temporal del algoritmo, antes y después de aplicar la poda](#análisis-de-complejidad-temporal-del-algoritmo-antes-y-después-de-aplicar-la-poda)
- [Notas adicionales](#notas-adicionales)
- [Agradecimientos](#agradecimientos)

---

## Descripción

El objetivo de este juego es conectar cuatro fichas consecutivas en el tablero antes que tu oponente, ya sea en forma horizontal, vertical o diagonal. Compite contra una IA desafiante o simplemente disfruta el juego en solitario.

---

## Cómo jugar

1. Al iniciar el programa, se imprimirá el tablero vacío.  
2. El jugador y la IA alternan turnos colocando fichas en las columnas del tablero. El jugador es él que da el primer movimiento.  
3. Durante tu turno, selecciona una columna ingresando un número del 1 al 7.  
4. Gana el primero que logre conectar 4 fichas consecutivas en cualquier dirección disponible.  
5. ¡Cuidado! La IA intentará bloquear tus jugadas y ganar estratégicamente. Usa tu cerebro si tienes para intentar ganar.

---

## Características

- **Tablero dinámico:** Representación visual del estado actual del tablero.
- **Algoritmo Minimax:** IA optimizada para buscar las mejores jugadas con poda alfa-beta.
- **Detección de bloqueos:** La IA anticipa y bloquea tus intentos de ganar.
- **Interfaz intuitiva:** Simple y fácil de entender para cualquier jugador.
- **Sistema de evaluación:** La IA evalúa cada estado del tablero para maximizar sus posibilidades de ganar.

---
## Requisitos

Para compilar y ejecutar este proyecto, necesitas:  

1. **Un compilador de C++** compatible con C++11 o superior, como `g++`.  
2. Un terminal para ejecutar el programa. Preferible que sea GitBash.

---
## Instrucciones para ejecutar el programa

1. Clona el repositorio en tu máquina local (solamente la primera vez):
    ```bash
    git clone https://github.com/MauConfort/Taller2C-.git
    ```
2. Ingresa al directorio del proyecto (si ya se realizó el primer paso antes, ir directamente a este):
    ```bash
    cd Taller2C-
    ```
3. Compila el código:
    ```bash
    g++ -o conectaCuatro src/ConectaCuatro.cpp
    ```
4. Ejecuta el programa (al terminar la partida, escribir el mismo para empezar otra nueva):
    ```bash
    ./conectaCuatro
    ```
## Algoritmo Minimax

El algoritmo **Minimax** es un algoritmo de búsqueda utilizado en juegos de dos jugadores, donde un jugador intenta maximizar su puntuación (IA) mientras que el otro jugador (humano) intenta minimizar la puntuación del oponente. En este caso, la IA juega para maximizar su puntuación y bloquear las jugadas del jugador.

## Poda Alfa-Beta

La **poda alfa-beta** es una optimización del algoritmo Minimax que mejora la eficiencia al reducir la cantidad de nodos evaluados en el árbol de decisiones. Utiliza dos valores, **alfa** y **beta**, para descartar ramas del árbol que no pueden influir en la decisión final.

## Explicación del juego

- **Objetivo**: El objetivo es ser el primero en conectar 4 fichas en línea (horizontal, vertical o diagonal).
- **Movimiento**: En cada turno, el jugador o la IA deben elegir una columna entre 1 y 7. Las fichas se colocan en la columna seleccionada, y la primera fila disponible se llena con la ficha.

## Diagrama del árbol de juego para un caso simple (ejemplo: primeros 3 movimientos)

Aquí se muestra un ejemplo básico de cómo se construye el árbol de decisiones para los primeros movimientos en el juego.
![Diagrama de árbol con primeros movimientos](Arbol_Codigo.png)

Este árbol ilustra cómo se evaluan las decisiones posibles tanto para el jugador como para la IA en función de la situación del tablero.

## Análisis de complejidad temporal del algoritmo, antes y después de aplicar la poda

### Complejidad temporal sin poda

El algoritmo Minimax tiene una complejidad de `O(b^d)`, donde `b` es el número de movimientos posibles en cada turno y `d` es la profundidad máxima del árbol de decisiones. Para juegos complejos, esta complejidad puede hacer que la ejecución sea lenta, ya que el árbol crece exponencialmente.

### Complejidad temporal con poda alfa-beta

La **poda alfa-beta** mejora significativamente la eficiencia del algoritmo, reduciendo la complejidad temporal a `O(b^(d/2))`. Esto se debe a que la poda descarta ramas que no pueden afectar el resultado del juego, evitando la exploración innecesaria de nodos y permitiendo que el algoritmo sea más rápido.

## Notas adicionales

1. **Ingreso de movimientos**:
   - El programa ingresa sus movimientos directamente desde la consola. Solo es necesario introducir el número de la columna (entre el 1 y 7) en la que deseas colocar tu ficha.
2. **Interfaz del juego**:
   - El tablero se representa visualmente en la consola. Las fichas de los jugadores están representadas por "O" (jugador) y "X" (IA). El jugador selecciona las columnas en las que quiere colocar sus fichas. Después de cada turno, se muestra el estado actualizado del tablero.
3. **Reglas para seleccionar columnas**:
   - Solo se puede elegir columnas válidas (entre 1 y 7) que tengan espacio disponible. Si seleccionas una columna llena, se te pedirá que elijas nuevamente. Lo mismo sucederá con la IA.
4. **Instrucciones**:
   - El jugador puede elegir una columna entre 1 y 7 para colocar su ficha.
   - La IA juega utilizando el algoritmo Minimax con poda alfa-beta.

## Agradecimientos

Gracias por utilizar este programa que ahora compila. ¡Diviértete jugando a Conecta Cuatro!
(PD: Originalmente esto era un codigo beta, pero por falta para configurar todo en mi GitHub institucional, lo tuve que mejorar en esta cuenta personal. Espero que no sea molestia, y además lo hice solo en Eclipse pero tuve que cambiar cosas :3).

