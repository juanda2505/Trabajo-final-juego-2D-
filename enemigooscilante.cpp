#include "enemigooscilante.h"
#include "proyectilrebote.h"
#include <cmath>   // std::sin

EnemigoOscilante::EnemigoOscilante(double x, double y, double amp, double om)
    : Enemigo(x, y), origenX(x), amplitud(amp), omega(om) {}

void EnemigoOscilante::moverse()
{
    paso++;
    x = origenX + amplitud * std::sin(omega * paso);   // x es protected en Entidad
    mover(0, 2);                                       // baja 2 px por paso
}

// La bala sale hacia el centro de la pantalla y rebota en las paredes
Proyectil* EnemigoOscilante::crearBala(double bx, double by) const
{
    double dir = (x < 400) ? VELOCIDAD_BALA : -VELOCIDAD_BALA;
    return new ProyectilRebote(bx, by, dir, 3, GRAVEDAD_BALA, REBOTES_BALA);
}
