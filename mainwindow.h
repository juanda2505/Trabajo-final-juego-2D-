#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include <QKeyEvent>
#include <vector>

#include "jugador.h"
#include "proyectil.h"
#include "entidadgrafica.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/*
 * MAINWINDOW (la ventana principal)
 * Es el "director" del juego: crea la escena, lee el teclado
 * y, cada pocos milisegundos, actualiza todo lo que pasa en pantalla.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT   // necesario para que Qt pueda usar señales y slots (el timer)

public:
    MainWindow(QWidget *parent = nullptr);   // prepara todo al abrir el juego
    ~MainWindow();                           // limpia la memoria al cerrar

protected:
    // Qt llama a estas dos funciones solo cuando se presiona o se suelta una tecla
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private slots:
    // Se ejecuta cada 16 ms (unas 60 veces por segundo): es el "latido" del juego
    void actualizar();

private:
    void disparar();   // crea una bala nueva saliendo de Goku

    Ui::MainWindow *ui;
    QGraphicsScene *scene;   // la escena: el "escenario" donde viven los objetos
    QGraphicsView *view;     // la vista: la ventana que deja ver la escena
    QTimer *timer;           // el reloj que llama a actualizar() una y otra vez

    Jugador *goku;                      // Goku (parte lógica)
    EntidadGrafica *gokuGrafico;        // Goku (su dibujo)
    std::vector<EntidadGrafica*> balas; // lista de todas las balas que hay en pantalla
    QPixmap spriteBala;                 // la imagen que usan todas las balas

    bool izquierda = false;   // true mientras la flecha izquierda esté presionada
    bool derecha = false;     // true mientras la flecha derecha esté presionada
};

#endif // MAINWINDOW_H
