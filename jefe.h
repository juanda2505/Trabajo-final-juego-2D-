#ifndef JEFE_H
#define JEFE_H

#include "enemigo.h"

/*
 * JEFE (galeón insignia)
 * Hereda de Enemigo, así que tiene vida, movimiento y disparo propios.
 * Se mueve de lado a lado en la parte alta y no baja.
 * Dispara en abanico: tres balas, o cinco en la segunda fase.
 */
class Jefe : public Enemigo {
    int direccion = 1;     // 1 = derecha, -1 = izquierda
    bool furia = false;    // segunda fase: cuando le queda la mitad de la vida
public:
    Jefe(double x, double y);

    void moverse() override;
    void revisarBorde(double maxX, double ancho) override;
    void disparar(std::vector<Proyectil*> &salida, double x, double y) const override;
    bool esJefe() const override { return true; }
};

#endif