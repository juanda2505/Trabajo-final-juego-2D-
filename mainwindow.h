#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QTimer>
#include <QKeyEvent>
#include <QPixmap>
#include <QString>
#include <vector>

#include "jugador.h"
#include "proyectil.h"
#include "enemigo.h"
#include "enemigooscilante.h"
#include "jefe.h"
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
    void actualizar();

private:
    void disparar();
    void agregarProyectil(Proyectil *p, const QPixmap &sprite);
    void agregarBalaEnemiga(Proyectil *p, const QPixmap &sprite);
    void crearEnemigo(double x, double y);
    void crearEnemigoOscilante(double x, double y);
    void crearJefe();
    void eliminar(EntidadGrafica *g);
    void sumarMuerte(Enemigo *e);
    void terminar(const QString &motivo);

    // Rayo Solar
    void intentarRayo();     // tecla C: empieza a cargar si hay puntos suficientes
    void actualizarRayo();   // avanza carga, espera y dibujo
    void dispararRayo();     // daña a los enemigos de la columna del peleador

    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QGraphicsView *view;
    QTimer *timer;

    Jugador *goku;
    EntidadGrafica *gokuGrafico;

    std::vector<EntidadGrafica*> balas;
    std::vector<EntidadGrafica*> balasEnemigas;
    std::vector<EntidadGrafica*> enemigosGraficos;

    QPixmap spriteBala;
    QPixmap spriteBalaEnemiga;
    QPixmap spriteBalaJefe;
    QPixmap framesEnemigo[2];
    QPixmap framesOscilante[2];
    QPixmap framesJefe[2];

    bool izquierda = false;
    bool derecha = false;
    bool juegoTerminado = false;
    bool jefeCreado = false;
    bool jefeDerrotado = false;
    int puntos = 0;
    int contadorSpawn = 0;
    int ticksJefe = 0;

    // Estado del Rayo Solar (1 s = 60 ticks, 8 s = 500 ticks)
    bool cargandoRayo = false;
    int cargaRayo = 0;
    int enfriamientoRayo = 0;
    int ticksVisualRayo = 0;
    QGraphicsRectItem *visualRayo = nullptr;
};

#endif // MAINWINDOW_H