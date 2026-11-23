#include "mainwindow.h"
#include "ui_mainwindow.h"

// CONSTRUCTOR: se ejecuta una sola vez, al abrir el juego.
// Aquí se crea todo lo que el juego necesita para empezar.
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 1. Crear el escenario (800 x 600) y la ventana que lo muestra
    scene = new QGraphicsScene(0, 0, 800, 600, this);
    view = new QGraphicsView(scene, this);
    setCentralWidget(view);
    resize(820, 620);

    // La vista no debe quedarse con el teclado ni usar las flechas para hacer scroll
    view->setFocusPolicy(Qt::NoFocus);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // La ventana principal es la que recibe el teclado
    setFocusPolicy(Qt::StrongFocus);
    setFocus();

    // 2. Cargar la imagen de Goku.
    //    Si todavía no existe, se usa un cuadro naranja de prueba.
    QPixmap sprite(":/sprites/goku.png");
    if (sprite.isNull()) {
        sprite = QPixmap(16, 8);
        sprite.fill(QColor("orange"));
    }

    // 3. Crear la imagen de las balas (un rectángulo amarillo de prueba)
    spriteBala = QPixmap(2, 6);
    spriteBala.fill(Qt::yellow);

    // 4. Crear a Goku (lógica) y su dibujo, y ponerlo en el escenario
    goku = new Jugador(380, 540);
    gokuGrafico = new EntidadGrafica(goku, sprite);
    scene->addItem(gokuGrafico);

    // 5. Crear el reloj del juego: cada 16 ms llama a actualizar()
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::actualizar);
    timer->start(16);
}

// DESTRUCTOR: se ejecuta al cerrar el juego.
// Todo lo creado con "new" debe borrarse a mano con "delete" para no dejar
// memoria ocupada. (Los dibujos los libera la escena sola.)
MainWindow::~MainWindow()
{
    for (EntidadGrafica *g : balas)
        delete g->getLogica();   // borra la parte lógica de cada bala
    delete goku;                 // borra a Goku
    delete ui;
}

// ACTUALIZAR: el "latido" del juego, se repite unas 60 veces por segundo.
// En cada latido se mueve todo un poquito y se redibuja.
void MainWindow::actualizar()
{
    // --- Goku ---
    // Calcula el ancho real de Goku en pantalla para que no se salga por la derecha
    double ancho = gokuGrafico->boundingRect().width() * gokuGrafico->scale();
    goku->actualizar(izquierda, derecha, 800 - ancho);   // mueve su parte lógica
    gokuGrafico->sincronizar();                          // mueve su dibujo igual

    // --- Balas ---
    // Se recorre la lista de balas una por una
    for (auto it = balas.begin(); it != balas.end(); ) {
        // Obtiene la parte lógica de la bala actual
        Proyectil *p = static_cast<Proyectil*>((*it)->getLogica());
        p->actualizar();        // la bala avanza
        (*it)->sincronizar();   // su dibujo avanza con ella

        if (p->fueraDePantalla()) {
            // La bala salió de la pantalla: se quita del escenario y se borra
            // de la memoria. 'erase' también la saca de la lista.
            scene->removeItem(*it);
            delete p;
            delete *it;
            it = balas.erase(it);
        } else {
            ++it;   // si sigue en pantalla, pasa a la siguiente bala
        }
    }

    // Aquí irán después: enemigos, colisiones, agente...
}

// DISPARAR: crea una bala nueva justo encima de Goku, en el centro
void MainWindow::disparar()
{
    double ancho = gokuGrafico->boundingRect().width() * gokuGrafico->scale();
    Proyectil *p = new Proyectil(goku->getX() + ancho / 2 - 3, goku->getY() - 20);
    EntidadGrafica *g = new EntidadGrafica(p, spriteBala, 3);
    scene->addItem(g);     // la pone en el escenario para que se vea
    balas.push_back(g);    // la agrega a la lista para poder moverla después
}

// TECLA PRESIONADA: Qt avisa cada vez que se aprieta una tecla
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    // Si la tecla se repite por mantenerla apretada, se ignora
    // (así un disparo equivale a una sola pulsación)
    if (event->isAutoRepeat()) return;

    if (event->key() == Qt::Key_Left)  izquierda = true;   // empieza a ir a la izquierda
    if (event->key() == Qt::Key_Right) derecha = true;     // empieza a ir a la derecha
    if (event->key() == Qt::Key_Space) disparar();         // espacio = disparar
}

// TECLA SOLTADA: Qt avisa cuando se suelta una tecla
void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat()) return;

    if (event->key() == Qt::Key_Left)  izquierda = false;  // deja de ir a la izquierda
    if (event->key() == Qt::Key_Right) derecha = false;    // deja de ir a la derecha
}