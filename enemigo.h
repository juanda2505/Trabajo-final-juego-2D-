#ifndef ENEMIGO_H
#define ENEMIGO_H

#include "entidad.h"
#include "proyectil.h"
#include <vector>

/*
 * ENEMIGO
 * Hereda de Entidad (lógica). Cada 'cadencia' ticks se mueve y cambia de cuadro.
 * Cada 'cadenciaDisparo' ticks puede disparar. No sabe nada de Qt.
 * Los métodos virtuales permiten que las clases hijas cambien su comportamiento.
 */
class Enemigo : public Entidad {
    int vel;               // píxeles por paso
    int cadencia;          // ticks entre pasos
    int timer = 0;
    int frame = 0;         // cuadro de animación: 0 o 1
    int cadenciaDisparo;   // ticks entre disparos
    int timerDisparo = 0;
public:
    Enemigo(double x, double y, int vida = 1, int vel = 12,
            int cadencia = 8, int cadenciaDisparo = 120);

    // Avanza un tick. Devuelve true si cambió de cuadro
    bool actualizar();

    // Movimiento de un paso. Las hijas pueden reemplazarlo
    virtual void moverse();

    // Devuelve true cuando el enemigo debe disparar
    bool listoParaDisparar();

    // Si va hacia un borde y lo toca, cambia de dirección
    virtual void revisarBorde(double maxX, double ancho);

    // Crea la bala que dispara el enemigo
    virtual Proyectil* crearBala(double x, double y) const;

    // Dispara y añade las balas a 'salida'. Por defecto, una sola bala.
    // El jefe la redefine para disparar en abanico.
    virtual void disparar(std::vector<Proyectil*> &salida, double x, double y) const;

    virtual bool esOscilante() const { return false; }
    virtual bool esJefe() const { return false; }

    // Dificultad: reduce cadencias, con mínimos para que siga siendo jugable
    void aumentarDificultad();

    int getFrame() const { return frame; }

private:
    void cambiarDireccion();
};

#endif