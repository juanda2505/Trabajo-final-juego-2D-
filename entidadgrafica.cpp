#include "entidadgrafica.h"

EntidadGrafica::EntidadGrafica(Entidad *e, const QPixmap &sprite)
    : QGraphicsPixmapItem(sprite), logica(e)
{
    setScale(3);                     // equivale a tu setScale(3,3)
    setPos(e->getX(), e->getY());
}

void EntidadGrafica::sincronizar()
{
    setPos(logica->getX(), logica->getY());
}