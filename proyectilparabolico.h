#ifndef PROYECTILPARABOLICO_H
#define PROYECTILPARABOLICO_H

#include "proyectil.h"

/*
 * PROYECTIL PARABÓLICO (primera física del juego)
 * Es la bala de cañón de los españoles. Sale con una velocidad inicial
 * y la gravedad la va curvando: sube, se frena y cae, dibujando una parábola.
 *   - Horizontal: la velocidad vx no cambia.
 *   - Vertical:   en cada paso se le suma la gravedad g a vy.
 */
class ProyectilParabolico : public Proyectil {
    double g;   // gravedad: cuánto aumenta vy en cada paso (más grande = cae más rápido)
public:
    // vx y vy son la velocidad inicial. La gravedad por defecto es 0.25
    ProyectilParabolico(double x, double y, double vx, double vy, double g = 0.25);

    // Mueve la bala aplicando la gravedad (cambia el comportamiento del padre)
    void actualizar() override;
};

#endif