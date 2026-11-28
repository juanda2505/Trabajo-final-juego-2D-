#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QPainter>
#include <QRandomGenerator>
#include <algorithm>   // std::clamp, std::remove
#include <cmath>       // std::abs

// Tiempo máximo para derrotar al jefe: 3750 ticks ≈ 60 s
static const int TIEMPO_JEFE = 3750;

// Puntos necesarios para usar el Rayo Solar y para que aparezca el jefe
static const int PUNTOS_RAYO = 15;
static const int PUNTOS_JEFE = 30;

// CONSTRUCTOR
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 1. Escenario de 800 x 600 y la vista que lo muestra
    scene = new QGraphicsScene(0, 0, 800, 600, this);
    QPixmap fondo(":/sprites/fondo_mar_tierra.png");
    if (!fondo.isNull())
        scene->setBackgroundBrush(fondo);
    view = new QGraphicsView(scene, this);
    setCentralWidget(view);
    resize(820, 620);

    view->setFocusPolicy(Qt::NoFocus);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setFocusPolicy(Qt::StrongFocus);
    setFocus();

    // 2. Peleador
    QPixmap sprite(":/sprites/personaje_capitan.png");
    if (sprite.isNull()) {
        sprite = QPixmap(16, 16);
        sprite.fill(QColor("orange"));
    }

    // 3. Balas: del peleador (azules), de los barcos (rojas) y del jefe (naranjas)
    spriteBala = QPixmap(3, 7);
    spriteBala.fill(QColor(0, 110, 255));
    {
        QPainter pb(&spriteBala);
        pb.fillRect(1, 0, 1, 7, QColor(180, 230, 255));
    }

    spriteBalaEnemiga = QPixmap(4, 6);
    spriteBalaEnemiga.fill(Qt::red);

    spriteBalaJefe = QPixmap(5, 7);
    spriteBalaJefe.fill(QColor(255, 150, 0));

    // 4. Barcos normales: dos cuadros de 16x12 separados por 1 px
    QPixmap hoja(":/sprites/enemigo_barco.png");
    if (hoja.isNull()) {
        hoja = QPixmap(33, 12);
        hoja.fill(Qt::magenta);
    }
    framesEnemigo[0] = hoja.copy(QRect(0, 0, 16, 12));
    framesEnemigo[1] = hoja.copy(QRect(17, 0, 16, 12));

    // 5. Barcos oscilantes: mismo formato
    QPixmap hojaOsc(":/sprites/enemigo_oscilante.png");
    if (hojaOsc.isNull()) {
        hojaOsc = QPixmap(33, 12);
        hojaOsc.fill(Qt::magenta);
    }
    framesOscilante[0] = hojaOsc.copy(QRect(0, 0, 16, 12));
    framesOscilante[1] = hojaOsc.copy(QRect(17, 0, 16, 12));

    // 6. Galeón jefe: dos cuadros de 32x16 separados por 1 px
    QPixmap hojaJefe(":/sprites/enemigo_jefe.png");
    if (hojaJefe.isNull()) {
        hojaJefe = QPixmap(65, 16);
        hojaJefe.fill(Qt::magenta);
    }
    framesJefe[0] = hojaJefe.copy(QRect(0, 0, 32, 16));
    framesJefe[1] = hojaJefe.copy(QRect(33, 0, 32, 16));

    // 7. Peleador (lógica y dibujo)
    goku = new Jugador(380, 540);   // el nombre de la variable se mantiene
    gokuGrafico = new EntidadGrafica(goku, sprite);
    scene->addItem(gokuGrafico);

    // 8. Primera fila de barcos
    for (int i = 0; i < 4; ++i) {
        if (i % 2 == 1)
            crearEnemigoOscilante(200 + i * 100, 50);
        else
            crearEnemigo(50 + i * 150, 50);
    }

    // 9. Reloj del juego
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::actualizar);
    timer->start(16);

    setWindowTitle(QString("Puntos: 0   Vidas: %1").arg(goku->getVida()));
}

// DESTRUCTOR: borra la parte lógica de todo lo creado con "new"
MainWindow::~MainWindow()
{
    for (EntidadGrafica *g : balas)            delete g->getLogica();
    for (EntidadGrafica *g : balasEnemigas)    delete g->getLogica();
    for (EntidadGrafica *g : enemigosGraficos) delete g->getLogica();
    delete goku;
    delete ui;
}

// ELIMINAR: quita el dibujo de la escena y borra la lógica
void MainWindow::eliminar(EntidadGrafica *g)
{
    scene->removeItem(g);
    delete g->getLogica();
    delete g;
}

// SUMAR MUERTE: da los puntos y marca la victoria si el que murió es el jefe
void MainWindow::sumarMuerte(Enemigo *e)
{
    if (e->esJefe()) {
        jefeDerrotado = true;
        puntos += 10;
    } else {
        puntos += e->esOscilante() ? 2 : 1;
    }
}

// CREAR BARCO normal
void MainWindow::crearEnemigo(double x, double y)
{
    Enemigo *e = new Enemigo(x, y);
    EntidadGrafica *g = new EntidadGrafica(e, framesEnemigo[0], 3);
    scene->addItem(g);
    enemigosGraficos.push_back(g);
}

// CREAR BARCO oscilante
void MainWindow::crearEnemigoOscilante(double x, double y)
{
    EnemigoOscilante *e = new EnemigoOscilante(x, y);
    EntidadGrafica *g = new EntidadGrafica(e, framesOscilante[0], 3);
    scene->addItem(g);
    enemigosGraficos.push_back(g);
}

// CREAR JEFE: aparece arriba al centro y solo sale una vez
void MainWindow::crearJefe()
{
    Jefe *j = new Jefe(352, 30);
    EntidadGrafica *g = new EntidadGrafica(j, framesJefe[0], 3);
    scene->addItem(g);
    enemigosGraficos.push_back(g);
    jefeCreado = true;
}

// ACTUALIZAR: el latido del juego, unas 60 veces por segundo
void MainWindow::actualizar()
{
    if (juegoTerminado) return;

    // --- Peleador ---
    double anchoGoku = gokuGrafico->boundingRect().width() * gokuGrafico->scale();
    goku->actualizar(izquierda, derecha, 800 - anchoGoku);
    gokuGrafico->sincronizar();

    // --- Rayo Solar: carga, espera y daño ---
    actualizarRayo();

    // --- Balas del peleador: avanzan y revisan si golpean a un enemigo ---
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
                    sumarMuerte(e);
                    eliminar(*ie);
                    enemigosGraficos.erase(ie);   // el iterador ie ya no es válido: salimos del ciclo
                }
                golpeo = true;
                break;
            }
        }

        if (golpeo || p->fueraDePantalla()) {
            eliminar(*it);
            it = balas.erase(it);
        } else {
            ++it;
        }
    }

    // Si el jefe murió (por bala o por el rayo), se gana la partida
    if (jefeDerrotado) {
        terminar("¡Victoria! Derrotaste al galeón");
        return;
    }

    // --- Enemigos: se mueven, cambian de cuadro, rebotan y disparan ---
    for (EntidadGrafica *g : enemigosGraficos) {
        Enemigo *e = static_cast<Enemigo*>(g->getLogica());
        double anchoE = g->boundingRect().width() * g->scale();
        double altoE  = g->boundingRect().height() * g->scale();

        if (e->actualizar()) {
            const QPixmap *frames = e->esJefe() ? framesJefe
                                                : (e->esOscilante() ? framesOscilante : framesEnemigo);
            g->setPixmap(frames[e->getFrame()]);
        }
        e->revisarBorde(800, anchoE);
        g->sincronizar();

        if (e->listoParaDisparar()) {
            std::vector<Proyectil*> nuevas;
            e->disparar(nuevas, e->getX() + anchoE / 2 - 2, e->getY() + altoE);
            const QPixmap &sprite = e->esJefe() ? spriteBalaJefe : spriteBalaEnemiga;
            for (Proyectil *p : nuevas)
                agregarBalaEnemiga(p, sprite);
        }

        // Si un barco llega al suelo, se pierde la partida
        if (e->getY() > 500) {
            terminar("Los barcos llegaron al suelo");
            return;
        }
    }

    // --- Balas enemigas: avanzan y revisan si golpean al peleador ---
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
        terminar("El peleador fue derrotado");
        return;
    }

    // --- Jefe: aparece al llegar a 30 puntos y tiene tiempo limitado ---
    if (!jefeCreado && puntos >= PUNTOS_JEFE)
        crearJefe();

    if (jefeCreado) {
        ticksJefe++;
        if (ticksJefe >= TIEMPO_JEFE) {
            terminar("Se acabó el tiempo: el galeón escapó");
            return;
        }
    }

    // --- Aparición de barcos: solo mientras no esté el jefe ---
    contadorSpawn++;
    if (!jefeCreado && contadorSpawn >= 120 && enemigosGraficos.size() < 8) {
        double x = -1;
        for (int intento = 0; intento < 10 && x < 0; ++intento) {
            double candidato = QRandomGenerator::global()->bounded(640) + 40;
            bool libre = true;
            for (EntidadGrafica *g : enemigosGraficos) {
                Entidad *otro = g->getLogica();
                if (otro->getY() < 150 && std::abs(otro->getX() - candidato) < 80)
                    libre = false;
            }
            if (libre)
                x = candidato;
        }

        if (x >= 0) {
            if (QRandomGenerator::global()->bounded(3) == 0)
                crearEnemigoOscilante(std::clamp(x, 180.0, 620.0), 50);
            else
                crearEnemigo(x, 50);
            contadorSpawn = 0;
        }
    }

    // --- Título: puntos, vidas, tiempo del jefe y estado del rayo ---
    QString titulo = QString("Puntos: %1   Vidas: %2").arg(puntos).arg(goku->getVida());
    if (jefeCreado)
        titulo += QString("   Tiempo jefe: %1 s").arg((TIEMPO_JEFE - ticksJefe) * 16 / 1000);
    if (cargandoRayo)
        titulo += "   Rayo: cargando";
    else if (enfriamientoRayo > 0)
        titulo += QString("   Rayo: espera %1 s").arg(enfriamientoRayo / 60 + 1);
    else if (puntos >= PUNTOS_RAYO)
        titulo += "   Rayo: listo (C)";
    setWindowTitle(titulo);
}

// AGREGAR BALA del peleador (suben)
void MainWindow::agregarProyectil(Proyectil *p, const QPixmap &sprite)
{
    EntidadGrafica *g = new EntidadGrafica(p, sprite, 3);
    scene->addItem(g);
    balas.push_back(g);
}

// AGREGAR BALA enemiga: recibe la bala ya creada y el sprite que le toca
void MainWindow::agregarBalaEnemiga(Proyectil *p, const QPixmap &sprite)
{
    EntidadGrafica *g = new EntidadGrafica(p, sprite, 3);
    scene->addItem(g);
    balasEnemigas.push_back(g);
}

// DISPARAR: bala del peleador justo encima de él
void MainWindow::disparar()
{
    double ancho = gokuGrafico->boundingRect().width() * gokuGrafico->scale();
    agregarProyectil(new Proyectil(goku->getX() + ancho / 2 - 3, goku->getY() - 20),
                     spriteBala);
}

// RAYO SOLAR, paso 1: al presionar C empieza la carga, si hay puntos y no está en espera
void MainWindow::intentarRayo()
{
    if (puntos >= PUNTOS_RAYO && !cargandoRayo && enfriamientoRayo == 0) {
        cargandoRayo = true;
        cargaRayo = 0;
    }
}

// RAYO SOLAR, paso 2: se llama en cada tick
void MainWindow::actualizarRayo()
{
    if (enfriamientoRayo > 0)
        enfriamientoRayo--;

    // El dibujo del rayo dura 20 ticks (unos 0.3 s)
    if (visualRayo && ++ticksVisualRayo >= 20) {
        scene->removeItem(visualRayo);
        delete visualRayo;
        visualRayo = nullptr;
    }

    // Terminada la carga de 1 s, el rayo se dispara
    if (cargandoRayo && ++cargaRayo >= 60) {
        cargandoRayo = false;
        enfriamientoRayo = 500;
        dispararRayo();
    }
}

// RAYO SOLAR, paso 3: daña a los enemigos que estén en la columna del peleador
void MainWindow::dispararRayo()
{
    double cx = gokuGrafico->x() + gokuGrafico->boundingRect().width() * gokuGrafico->scale() / 2;

    // Primero reúno los enemigos tocados, sin modificar la lista mientras la recorro
    std::vector<EntidadGrafica*> tocados;
    for (EntidadGrafica *g : enemigosGraficos) {
        double ancho = g->boundingRect().width() * g->scale();
        if (g->x() < cx + 20 && g->x() + ancho > cx - 20)
            tocados.push_back(g);
    }

    for (EntidadGrafica *g : tocados) {
        Enemigo *e = static_cast<Enemigo*>(g->getLogica());
        e->recibirDanio(3);
        if (!e->estaViva()) {
            sumarMuerte(e);
            // Primero se quita de la lista y después se borra: nunca queda un puntero colgando
            enemigosGraficos.erase(std::remove(enemigosGraficos.begin(),
                                               enemigosGraficos.end(), g),
                                   enemigosGraficos.end());
            eliminar(g);
        }
    }

    // Dibujo provisional: una franja amarilla de 40 px sobre el peleador
    visualRayo = scene->addRect(cx - 20, 0, 40, 600,
                                QPen(Qt::NoPen),
                                QBrush(QColor(255, 230, 120, 200)));
    ticksVisualRayo = 0;
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
    if (event->key() == Qt::Key_C)     intentarRayo();

    // PRUEBA TEMPORAL: la tecla D sube la dificultad de los barcos
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