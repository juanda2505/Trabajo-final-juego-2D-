#ifndef ENEMIGOOSCILANTE_H
#define ENEMIGOOSCILANTE_H

#include "enemigo.h"

/*
 * ENEMIGO OSCILANTE
 * Hereda de Enemigo y reemplaza el movimiento por una física oscilatoria:
 *     x(t) = origenX + A · sen(ω · t)
 *   A : amplitud, cuántos píxeles se aleja del centro
 *   ω : velocidad angular, en radianes por paso
 *   t : número de pasos transcurridos
 * Las constantes son los parámetros del modelo: cambiarlas cambia el
 * comportamiento sin tocar la lógica.
 */
class EnemigoOscilante : public Enemigo {
public:
    // Parámetros por defecto del movimiento
    static constexpr double AMPLITUD_DEF  = 150;    // A: píxeles de recorrido a cada lado
    static constexpr double FRECUENCIA_DEF = 0.05;  // ω: radianes por paso (un ciclo ≈ 125 pasos)

    // Parámetros de la bala que dispara
    static constexpr double VELOCIDAD_BALA = 5;     // vx de la bala, en px por paso
    static constexpr double GRAVEDAD_BALA  = 0.02;  // g de la bala
    static constexpr int    REBOTES_BALA   = 4;     // rebotes en las paredes antes de desaparecer

private:
    double origenX;    // centro de la oscilación
    double amplitud;   // A
    double omega;      // ω
    int paso = 0;      // t

public:
    EnemigoOscilante(double x, double y,
                     double amp = AMPLITUD_DEF, double om = FRECUENCIA_DEF);

    void moverse() override;                                  // física oscilatoria
    void revisarBorde(double, double) override {}             // la amplitud ya lo mantiene en pantalla
    Proyectil* crearBala(double x, double y) const override;  // bala que rebota
    bool esOscilante() const override { return true; }
};

#endif