#ifndef PROYECTILREBOTE_H
#define PROYECTILREBOTE_H

#include "proyectilparabolico.h"

/*
 * PROYECTIL QUE REBOTA
 * Hereda de ProyectilParabolico: tiene gravedad (física parabólica)
 * y además rebota en las paredes laterales, perdiendo energía en cada rebote.
 * No rebota en el suelo: si cae, sale de la pantalla.
 */
class ProyectilRebote : public ProyectilParabolico {
    int rebotesRestantes;   // cuántos rebotes le quedan
public:
    ProyectilRebote(double x, double y, double vx, double vy,
                    double g = 0.1, int rebotes = 3);

    void actualizar() override;
};

#endif