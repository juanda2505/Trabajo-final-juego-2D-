#ifndef ENTIDAD_H
#define ENTIDAD_H

/*
 * ENTIDAD
 * Es la "plantilla base" de todo lo que existe en el juego:
 * Goku, los enemigos, las balas...
 * Todos tienen una posición (x, y) y una cantidad de vida.
 * Las demás clases (Jugador, Proyectil...) se construyen a partir de esta,
 * por eso se dice que "heredan" de Entidad.
 */
class Entidad {
protected:
    // "protected" = lo pueden usar esta clase y las que heredan de ella
    double x, y;   // posición en la pantalla (x: horizontal, y: vertical)
    int vida;      // puntos de vida; si llega a 0, la entidad "muere"
public:
    // Constructor: se ejecuta al crear una entidad y le da sus valores iniciales
    Entidad(double x, double y, int vida);

    // Destructor virtual: permite borrar correctamente las clases que heredan
    virtual ~Entidad() = default;

    // Funciones "get": solo devuelven un dato, no lo cambian
    double getX() const { return x; }
    double getY() const { return y; }
    int getVida() const { return vida; }

    // Cambia la posición sumando dx (horizontal) y dy (vertical)
    void mover(double dx, double dy);

    // Resta puntos de vida (por ejemplo, cuando un golpe la alcanza)
    void recibirDanio(int d);

    // Devuelve true (verdadero) si todavía le queda vida
    bool estaViva() const { return vida > 0; }
};

#endif