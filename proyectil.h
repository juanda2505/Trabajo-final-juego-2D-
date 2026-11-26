#ifndef PROYECTIL_H
#define PROYECTIL_H

#include "entidad.h"

/*
 * PROYECTIL
 * Es una bala. Sirve para los disparos de Goku (suben) y para los de los
 * enemigos (bajan). Se mueve en línea recta según su velocidad.
 *   vx : velocidad horizontal (positiva = derecha, negativa = izquierda)
 *   vy : velocidad vertical   (positiva = baja,    negativa = sube)
 */
class Proyectil : public Entidad {
protected:
    // "protected" para que las clases que heredan (como la parabólica) las usen
    double vx, vy;
public:
    // Crea la bala en (x, y). Por defecto sube recta a velocidad 8
    Proyectil(double x, double y, double vx = 0, double vy = -8);

    // Avanza la bala un paso. Es "virtual": las clases hijas pueden
    // cambiar la forma de moverse (por ejemplo, con gravedad)
    virtual void actualizar();

    // Devuelve true si la bala salió por cualquier borde y ya se puede borrar
    bool fueraDePantalla() const;
};

#endif