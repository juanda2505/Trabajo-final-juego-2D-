#include "jugador.h"

// Crea a Goku: llama al constructor de Entidad con 3 vidas y velocidad 5
Jugador::Jugador(double x, double y) : Entidad(x, y, 3), vel(5) {}

// Se llama muchas veces por segundo.
// Si se presiona una flecha y todavía hay espacio, Goku se mueve 'vel' píxeles.
void Jugador::actualizar(bool izq, bool der, double maxX)
{
    if (der && x + vel <= maxX) mover(vel, 0);    // derecha, sin pasar el borde
    if (izq && x - vel >= 0)    mover(-vel, 0);   // izquierda, sin pasar el borde
}