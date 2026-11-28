#include "jefe.h"

// Vida 15, sin velocidad propia (usa moverse()), cadencia de paso 2 y disparo cada 60 ticks
Jefe::Jefe(double x, double y) : Enemigo(x, y, 15, 0, 2, 60) {}

void Jefe::moverse()
{
    if (!furia && getVida() <= 7)   // segunda fase al perder la mitad de la vida
        furia = true;

    double paso = furia ? 6 : 3;    // en la segunda fase se mueve más rápido
    mover(direccion * paso, 0);     // solo horizontal: el jefe no baja
}

// Rebota en los bordes sin bajar, a diferencia de los barcos normales
void Jefe::revisarBorde(double maxX, double ancho)
{
    if (x <= 0)              direccion = 1;
    if (x + ancho >= maxX)   direccion = -1;
}

// Abanico: una bala recta y dos inclinadas. En la segunda fase, dos más
void Jefe::disparar(std::vector<Proyectil*> &salida, double x, double y) const
{
    salida.push_back(new Proyectil(x, y, -2, 4));
    salida.push_back(new Proyectil(x, y, 0, 4));
    salida.push_back(new Proyectil(x, y, 2, 4));
    if (furia) {
        salida.push_back(new Proyectil(x, y, -4, 4));
        salida.push_back(new Proyectil(x, y, 4, 4));
    }
}