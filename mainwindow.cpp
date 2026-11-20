#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Escena y vista
    scene = new QGraphicsScene(0, 0, 800, 600, this);
    view = new QGraphicsView(scene, this);
    setCentralWidget(view);
    resize(820, 620);

    // Sprite (si no existe todavía, usa un cuadro naranja de prueba)
    QPixmap sprite(":/sprites/goku.png");
    if (sprite.isNull()) {
        sprite = QPixmap(30, 30);
        sprite.fill(Qt::red);
    }

    // Jugador: lógica + parte gráfica
    goku = new Jugador(380, 500);
    gokuGrafico = new EntidadGrafica(goku, sprite);
    scene->addItem(gokuGrafico);

    // Reloj del juego
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::actualizar);
    timer->start(16);
}

MainWindow::~MainWindow()
{
    delete goku;   // la entidad lógica la liberamos nosotros
    delete ui;     // la escena y la vista las libera Qt (tienen padre)
}

void MainWindow::actualizar()
{
    // Movimiento según las teclas presionadas
    if (izquierda) goku->mover(-5, 0);
    if (derecha)   goku->mover(5, 0);

    // Límites de la pantalla
    double ancho = gokuGrafico->boundingRect().width() * gokuGrafico->scale();
    if (goku->getX() < 0)
        goku->mover(-goku->getX(), 0);
    if (goku->getX() > 800 - ancho)
        goku->mover(800 - ancho - goku->getX(), 0);

    // Pasar la posición lógica al dibujo
    gokuGrafico->sincronizar();

    // Aquí irán después: enemigos, balas, colisiones, agente...
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat()) return;
    if (event->key() == Qt::Key_Left)  izquierda = true;
    if (event->key() == Qt::Key_Right) derecha = true;
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat()) return;
    if (event->key() == Qt::Key_Left)  izquierda = false;
    if (event->key() == Qt::Key_Right) derecha = false;
}