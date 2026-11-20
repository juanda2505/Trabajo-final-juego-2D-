#include "entidad.h"

Entidad::Entidad(double x, double y, int vida)
    : x(x), y(y), vida(vida) {}

void Entidad::mover(double dx, double dy) { x += dx; y += dy; }
void Entidad::recibirDanio(int d) { vida -= d; }