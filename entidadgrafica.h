#ifndef ENTIDADGRAFICA_H
#define ENTIDADGRAFICA_H

#include <QGraphicsPixmapItem>
#include "entidad.h"

class EntidadGrafica : public QGraphicsPixmapItem {
    Entidad *logica;   // no es dueña: la gestiona quien la creó
public:
    EntidadGrafica(Entidad *e, const QPixmap &sprite);
    void sincronizar();   // copia la posición lógica al dibujo
    Entidad *getLogica() const { return logica; }
};

#endif