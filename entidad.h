#ifndef ENTIDAD_H
#define ENTIDAD_H

class Entidad {
protected:
    double x, y;
    int vida;
public:
    Entidad(double x, double y, int vida);
    virtual ~Entidad() = default;

    double getX() const { return x; }
    double getY() const { return y; }
    int getVida() const { return vida; }

    void mover(double dx, double dy);
    void recibirDanio(int d);
    bool estaViva() const { return vida > 0; }
};

#endif