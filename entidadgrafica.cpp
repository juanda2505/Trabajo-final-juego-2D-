#include "entidadgrafica.h"

// Crea el dibujo con la imagen indicada, lo agranda según 'escala'
// y lo coloca donde está la entidad
EntidadGrafica::EntidadGrafica(Entidad *e, const QPixmap &sprite, double escala)
    : QGraphicsPixmapItem(sprite), logica(e)
{
    setScale(escala);                   // agranda el sprite (3 = tres veces más grande)
    setPos(e->getX(), e->getY());       // lo ubica en la posición inicial
}

// Pone el dibujo en el mismo lugar donde la lógica dice que está la entidad
void EntidadGrafica::sincronizar()
{
    setPos(logica->getX(), logica->getY());
}