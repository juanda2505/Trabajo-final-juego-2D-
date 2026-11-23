#ifndef PROYECTIL_H
#define PROYECTIL_H

#include "entidad.h"

/*
 * PROYECTIL
 * Es una bala (el disparo de Goku). También hereda de Entidad.
 * Se mueve sola en línea recta hacia arriba hasta salir de la pantalla.
 */
class Proyectil : public Entidad {
    double vy;   // velocidad vertical (negativa = sube, positiva = baja)
public:
    // Crea la bala en (x, y). Si no se indica 'vy', sube a velocidad -8
    Proyectil(double x, double y, double vy = -8);

    // Avanza la bala un paso según su velocidad
    void actualizar();

    // Devuelve true si la bala ya salió por arriba y se puede borrar
    bool fueraDePantalla() const;
};

#endif