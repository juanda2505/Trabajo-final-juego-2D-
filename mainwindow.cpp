#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QPainter>
#include <QRandomGenerator>

namespace {
// Pinta de color un sprite. Solo colorea donde ya había imagen,
// así el fondo transparente se conserva
QPixmap tintar(const QPixmap &original, const QColor &color)
{
    QPixmap resultado(original.size());
    resultado.fill(Qt::transparent);
    QPainter pintor(&resultado);
    pintor.drawPixmap(0, 0, original);
    pintor.setCompositionMode(QPainter::CompositionMode_SourceAtop);
    pintor.fillRect(resultado.rect(), color);
    pintor.end();
    return resultado;
}
}

// CONSTRUCTOR: se ejecuta una sola vez, al abrir el juego
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 1. Escenario de 800 x 600 y la vista que lo muestra
    scene = new QGraphicsScene(0, 0, 800, 600, this);
    view = new QGraphicsView(scene, this);
    setCentralWidget(view);
    resize(820, 620);

    view->setFocusPolicy(Qt::NoFocus);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setFocusPolicy(Qt::StrongFocus);
    setFocus();

    // 2. Sprites. Si una imagen no existe, se usa un cuadro de color de prueba
    QPixmap sprite(":/sprites/goku.png");
    if (sprite.isNull()) {
        sprite = QPixmap(16, 8);
        sprite.fill(QColor("orange"));
    }

    spriteBala = QPixmap(2, 6);
    spriteBala.fill(Qt::yellow);

    spriteBalaEnemiga = QPixmap(4, 6);
    spriteBalaEnemiga.fill(Qt::red);

    QPixmap hoja(":/sprites/enemigo.png");
    if (hoja.isNull()) {
        hoja = QPixmap(18, 8);
        hoja.fill(Qt::magenta);
    }
    // Cada cuadro mide 8x8 y están separados por 9 píxeles
    framesEnemigo[0] = hoja.copy(QRect(0, 0, 8, 8));
    framesEnemigo[1] = hoja.copy(QRect(9, 0, 8, 8));

    // Versión morada de los mismos cuadros para los enemigos oscilantes
    framesOscilante[0] = tintar(framesEnemigo[0], QColor(170, 70, 255));
    framesOscilante[1] = tintar(framesEnemigo[1], QColor(170, 70, 255));

    // 3. Goku (lógica y dibujo)
    goku = new Jugador(380, 540);
    gokuGrafico = new EntidadGrafica(goku, sprite);
    scene->addItem(gokuGrafico);

    // 4. Primera fila: alterna enemigos normales y oscilantes.
    //    Los oscilantes se colocan entre 200 y 500 para que, con amplitud 150,
    //    nunca salgan de la pantalla.
    for (int i = 0; i < 4; ++i) {
        if (i % 2 == 1)
            crearEnemigoOscilante(200 + i * 100, 50);
        else
            crearEnemigo(50 + i * 150, 50);
    }

    // 5. Reloj del juego
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::actualizar);
    timer->start(16);

    setWindowTitle(QString("Puntos: 0   Vidas: %1").arg(goku->getVida()));
}

// DESTRUCTOR: borra la parte lógica de todo lo creado con "new".
// Los dibujos los libera la escena cuando se destruye.
MainWindow::~MainWindow()
{
    for (EntidadGrafica *g : balas)            delete g->getLogica();
    for (EntidadGrafica *g : balasEnemigas)    delete g->getLogica();
    for (EntidadGrafica *g : enemigosGraficos) delete g->getLogica();
    delete goku;
    delete ui;
}

// ELIMINAR: quita el dibujo de la escena y borra la lógica.
// Gracias al destructor virtual de Entidad, delete borra la clase real
// aunque el puntero sea Entidad*.
void MainWindow::eliminar(EntidadGrafica *g)
{
    scene->removeItem(g);
    delete g->getLogica();
    delete g;
}

// CREAR ENEMIGO normal
void MainWindow::crearEnemigo(double x, double y)
{
    Enemigo *e = new Enemigo(x, y);
    EntidadGrafica *g = new EntidadGrafica(e, framesEnemigo[0], 3);
    scene->addItem(g);
    enemigosGraficos.push_back(g);
}

// CREAR ENEMIGO OSCILANTE (morado, se mueve de lado a lado)
void MainWindow::crearEnemigoOscilante(double x, double y)
{
    EnemigoOscilante *e = new EnemigoOscilante(x, y);
    EntidadGrafica *g = new EntidadGrafica(e, framesOscilante[0], 3);
    scene->addItem(g);
    enemigosGraficos.push_back(g);
}

// ACTUALIZAR: el latido del juego, unas 60 veces por segundo
void MainWindow::actualizar()
{
    if (juegoTerminado) return;

    // --- Goku ---
    double anchoGoku = gokuGrafico->boundingRect().width() * gokuGrafico->scale();
    goku->actualizar(izquierda, derecha, 800 - anchoGoku);
    gokuGrafico->sincronizar();

    // --- Balas de Goku: avanzan y revisan si golpean a un enemigo ---
    for (auto it = balas.begin(); it != balas.end(); ) {
        Proyectil *p = static_cast<Proyectil*>((*it)->getLogica());
        p->actualizar();
        (*it)->sincronizar();

        bool golpeo = false;
        for (auto ie = enemigosGraficos.begin(); ie != enemigosGraficos.end(); ++ie) {
            if ((*it)->collidesWithItem(*ie)) {
                Enemigo *e = static_cast<Enemigo*>((*ie)->getLogica());
                e->recibirDanio(1);
                if (!e->estaViva()) {
                    puntos += e->esOscilante() ? 2 : 1;   // el oscilante vale más
                    eliminar(*ie);
                    enemigosGraficos.erase(ie);
                }
                golpeo = true;
                break;   // el iterador ie ya no es válido: salimos del ciclo
            }
        }

        if (golpeo || p->fueraDePantalla()) {
            eliminar(*it);
            it = balas.erase(it);
        } else {
            ++it;
        }
    }

    // --- Enemigos: se mueven, cambian de cuadro, rebotan y disparan ---
    for (EntidadGrafica *g : enemigosGraficos) {
        Enemigo *e = static_cast<Enemigo*>(g->getLogica());
        double anchoE = g->boundingRect().width() * g->scale();
        double altoE  = g->boundingRect().height() * g->scale();

        if (e->actualizar()) {
            const QPixmap *frames = e->esOscilante() ? framesOscilante : framesEnemigo;
            g->setPixmap(frames[e->getFrame()]);
        }
        e->revisarBorde(800, anchoE);
        g->sincronizar();

        if (e->listoParaDisparar())
            agregarBalaEnemiga(e->crearBala(e->getX() + anchoE / 2 - 2, e->getY() + altoE));

        // Si un enemigo llega al suelo, se pierde la partida
        if (e->getY() > 500) {
            terminar("Los enemigos llegaron al suelo");
            return;
        }
    }

    // --- Balas enemigas: avanzan y revisan si golpean a Goku ---
    for (auto it = balasEnemigas.begin(); it != balasEnemigas.end(); ) {
        Proyectil *p = static_cast<Proyectil*>((*it)->getLogica());
        p->actualizar();
        (*it)->sincronizar();

        bool golpeo = (*it)->collidesWithItem(gokuGrafico);
        if (golpeo)
            goku->recibirDanio(1);

        if (golpeo || p->fueraDePantalla()) {
            eliminar(*it);
            it = balasEnemigas.erase(it);
        } else {
            ++it;
        }
    }

    if (!goku->estaViva()) {
        terminar("Goku fue derrotado");
        return;
    }

    // --- Aparición: cada ~3 s, hasta 8 enemigos. Un tercio son oscilantes ---
    contadorSpawn++;
    if (contadorSpawn >= 180 && enemigosGraficos.size() < 8) {
        if (QRandomGenerator::global()->bounded(3) == 0)
            crearEnemigoOscilante(180 + QRandomGenerator::global()->bounded(440), 50);
        else
            crearEnemigo(QRandomGenerator::global()->bounded(700), 50);
        contadorSpawn = 0;
    }

    setWindowTitle(QString("Puntos: %1   Vidas: %2").arg(puntos).arg(goku->getVida()));
}

// AGREGAR PROYECTIL: balas de Goku (suben)
void MainWindow::agregarProyectil(Proyectil *p, const QPixmap &sprite)
{
    EntidadGrafica *g = new EntidadGrafica(p, sprite, 3);
    scene->addItem(g);
    balas.push_back(g);
}

// AGREGAR BALA ENEMIGA: recibe la bala ya creada por el enemigo.
// Así MainWindow no necesita saber si es recta o rebota.
void MainWindow::agregarBalaEnemiga(Proyectil *p)
{
    EntidadGrafica *g = new EntidadGrafica(p, spriteBalaEnemiga, 3);
    scene->addItem(g);
    balasEnemigas.push_back(g);
}

// DISPARAR: bala de Goku justo encima de él
void MainWindow::disparar()
{
    double ancho = gokuGrafico->boundingRect().width() * gokuGrafico->scale();
    agregarProyectil(new Proyectil(goku->getX() + ancho / 2 - 3, goku->getY() - 20),
                     spriteBala);
}

// TERMINAR: detiene el juego y muestra el resultado
void MainWindow::terminar(const QString &motivo)
{
    juegoTerminado = true;
    timer->stop();
    QMessageBox::information(this, "Fin del juego",
                             QString("%1\nPuntos: %2").arg(motivo).arg(puntos));
    close();
}

// TECLA PRESIONADA
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat()) return;

    if (event->key() == Qt::Key_Left)  izquierda = true;
    if (event->key() == Qt::Key_Right) derecha = true;
    if (event->key() == Qt::Key_Space) disparar();

    // PRUEBA TEMPORAL: la tecla D sube la dificultad de los enemigos
    if (event->key() == Qt::Key_D)
        for (EntidadGrafica *g : enemigosGraficos)
            static_cast<Enemigo*>(g->getLogica())->aumentarDificultad();
}

// TECLA SOLTADA
void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat()) return;

    if (event->key() == Qt::Key_Left)  izquierda = false;
    if (event->key() == Qt::Key_Right) derecha = false;
}