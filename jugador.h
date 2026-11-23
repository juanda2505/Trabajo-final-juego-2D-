#ifndef JUGADOR_H
#define JUGADOR_H

#include "entidad.h"

/*
 * JUGADOR (Goku)
 * Hereda de Entidad, así que ya tiene posición y vida.
 * Aquí se agrega lo propio de Goku: su velocidad de movimiento.
 * Más adelante irán sus características especiales (veloz, fortachón...).
 */
class Jugador : public Entidad {
    int vel;   // velocidad: cuántos píxeles se mueve en cada "tic" del juego
public:
    // Crea a Goku en la posición (x, y) con 3 vidas
    Jugador(double x, double y);

    // Mueve a Goku según las teclas presionadas.
    //   izq / der : si la flecha izquierda / derecha está presionada
    //   maxX      : límite derecho de la pantalla para que no se salga
    void actualizar(bool izq, bool der, double maxX);
};

#endif