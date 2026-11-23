#ifndef ENTIDADGRAFICA_H
#define ENTIDADGRAFICA_H

#include <QGraphicsPixmapItem>
#include "entidad.h"

/*
 * ENTIDAD GRÁFICA
 * Es el "dibujo" de una Entidad. Une las dos capas del proyecto:
 *   - la parte lógica (Entidad): dónde está y cuánta vida tiene
 *   - la parte visual (Qt): la imagen que se ve en pantalla
 * Hereda de QGraphicsPixmapItem, la clase de Qt que sabe mostrar una imagen.
 */
class EntidadGrafica : public QGraphicsPixmapItem {
    Entidad *logica;   // la entidad a la que pertenece este dibujo
public:
    // Crea el dibujo: recibe la entidad, la imagen (sprite) y el tamaño (escala)
    EntidadGrafica(Entidad *e, const QPixmap &sprite, double escala = 3);

    // Copia la posición de la entidad al dibujo.
    // Hay que llamarla después de mover la entidad, si no la imagen no se mueve.
    void sincronizar();

    // Devuelve la entidad lógica asociada a este dibujo
    Entidad *getLogica() const { return logica; }
};

#endif