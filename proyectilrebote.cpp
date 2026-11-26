#include "proyectilrebote.h"

ProyectilRebote::ProyectilRebote(double x, double y, double vx, double vy,
                                 double g, int rebotes)
    : ProyectilParabolico(x, y, vx, vy, g), rebotesRestantes(rebotes) {}

void ProyectilRebote::actualizar()
{
    ProyectilParabolico::actualizar();    // gravedad + movimiento parabólico

    // Si toca una pared y le quedan rebotes, invierte la velocidad horizontal
    if (rebotesRestantes > 0 && (x < 0 || x > 800)) {
        vx = -vx * 0.9;                   // pierde 10% de su velocidad en cada rebote
        x = (x < 0) ? 0 : 800;            // la devuelve a la pared
        rebotesRestantes--;
    }
}