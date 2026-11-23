#include "entidad.h"

// Constructor: guarda la posición y la vida con las que nace la entidad
Entidad::Entidad(double x, double y, int vida)
    : x(x), y(y), vida(vida) {}

// Mueve la entidad: si dx = 5 se desplaza 5 a la derecha, si dx = -5 a la izquierda.
// Igual con dy: positivo baja, negativo sube (en pantalla el eje y crece hacia abajo).
void Entidad::mover(double dx, double dy) { x += dx; y += dy; }

// Le quita 'd' puntos de vida a la entidad
void Entidad::recibirDanio(int d) { vida -= d; }