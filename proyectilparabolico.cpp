#include "proyectilparabolico.h"

// Crea la bala llamando al constructor de Proyectil y guarda la gravedad
ProyectilParabolico::ProyectilParabolico(double x, double y,
                                         double vx, double vy, double g)
    : Proyectil(x, y, vx, vy), g(g) {}

// Cada paso: primero la gravedad cambia la velocidad vertical,
// luego se mueve la bala con las velocidades ya actualizadas
void ProyectilParabolico::actualizar()
{
    vy += g;          // la gravedad "tira" hacia abajo, poco a poco
    mover(vx, vy);    // avanza en horizontal (vx) y en vertical (vy)
}