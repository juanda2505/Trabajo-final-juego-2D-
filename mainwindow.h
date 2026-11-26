#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include <QKeyEvent>
#include <QPixmap>
#include <QString>
#include <vector>

#include "jugador.h"
#include "proyectil.h"
#include "enemigo.h"
#include "enemigooscilante.h"
#include "entidadgrafica.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/*
 * MAINWINDOW (la ventana principal)
 * Es el "director" del juego: crea la escena, lee el teclado,
 * revisa las colisiones y, cada 16 ms, actualiza todo lo que pasa.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private slots:
    void actualizar();   // se ejecuta cada 16 ms

private:
    void disparar();                                          // bala de Goku
    void agregarProyectil(Proyectil *p, const QPixmap &sprite);
    void agregarBalaEnemiga(Proyectil *p);                    // recibe la bala ya creada
    void crearEnemigo(double x, double y);
    void crearEnemigoOscilante(double x, double y);
    void eliminar(EntidadGrafica *g);                         // quita dibujo y lógica
    void terminar(const QString &motivo);                     // fin de la partida

    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QGraphicsView *view;
    QTimer *timer;

    Jugador *goku;
    EntidadGrafica *gokuGrafico;

    std::vector<EntidadGrafica*> balas;             // balas de Goku
    std::vector<EntidadGrafica*> balasEnemigas;     // balas de los enemigos
    std::vector<EntidadGrafica*> enemigosGraficos;  // enemigos en pantalla

    QPixmap spriteBala;
    QPixmap spriteBalaEnemiga;
    QPixmap framesEnemigo[2];      // cuadros de los enemigos normales
    QPixmap framesOscilante[2];    // cuadros de los enemigos oscilantes (morados)

    bool izquierda = false;
    bool derecha = false;
    bool juegoTerminado = false;
    int puntos = 0;
    int contadorSpawn = 0;
};

#endif // MAINWINDOW_H
