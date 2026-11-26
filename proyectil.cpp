#include "proyectil.h"

// Crea la bala con 1 de vida y las velocidades indicadas
Proyectil::Proyectil(double x, double y, double vx, double vy)
    : Entidad(x, y, 1), vx(vx), vy(vy) {}

// Mueve la bala en línea recta: suma su velocidad a la posición
void Proyectil::actualizar() { mover(vx, vy); }

// La pantalla mide 800 x 600. Si la bala pasó de esos bordes
// (con un margen de 50), ya no se ve ni sirve
bool Proyectil::fueraDePantalla() const
{
    return y < -50 || y > 650 || x < -50 || x > 850;
}