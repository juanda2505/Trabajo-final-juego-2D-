#include "proyectil.h"

// Crea la bala con 1 de vida y la velocidad indicada
Proyectil::Proyectil(double x, double y, double vy)
    : Entidad(x, y, 1), vy(vy) {}

// Mueve la bala solo en vertical, sumando su velocidad a la posición
void Proyectil::actualizar() { mover(0, vy); }

// Si la bala pasó del borde superior (y menor que -50), ya no se ve ni sirve
bool Proyectil::fueraDePantalla() const { return y < -50; }