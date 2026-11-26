#include "enemigo.h"
#include <cstdlib>      // std::abs
#include <stdexcept>    // std::invalid_argument

Enemigo::Enemigo(double x, double y, int vida, int vel, int cadencia, int cadenciaDisparo)
    : Entidad(x, y, vida), vel(vel), cadencia(cadencia), cadenciaDisparo(cadenciaDisparo)
{
    if (cadencia <= 0 || cadenciaDisparo <= 0)
        throw std::invalid_argument("Las cadencias del enemigo deben ser positivas");
}

bool Enemigo::actualizar()
{
    bool cambio = false;
    if (timer >= cadencia) {
        moverse();                    // llama a la versión de la clase real
        frame = (frame + 1) % 2;
        timer = 0;
        cambio = true;
    }
    timer++;
    return cambio;
}

// Movimiento base: pasos horizontales
void Enemigo::moverse()
{
    mover(vel, 0);
}

bool Enemigo::listoParaDisparar()
{
    timerDisparo++;
    if (timerDisparo >= cadenciaDisparo) {
        timerDisparo = 0;
        return true;
    }
    return false;
}

void Enemigo::cambiarDireccion()
{
    vel = -vel;
    mover(0, std::abs(vel));          // baja una fila
}

void Enemigo::revisarBorde(double maxX, double ancho)
{
    bool yendoDerecha   = vel > 0 && x + ancho > maxX;
    bool yendoIzquierda = vel < 0 && x < 0;
    if (yendoDerecha || yendoIzquierda)
        cambiarDireccion();
}

// Bala base: recta, hacia abajo
Proyectil* Enemigo::crearBala(double x, double y) const
{
    return new Proyectil(x, y, 0, 5);
}

void Enemigo::aumentarDificultad()
{
    if (cadencia > 6)           cadencia -= 2;
    if (cadenciaDisparo > 40)   cadenciaDisparo -= 10;
}