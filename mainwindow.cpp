#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsItem>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::start()
{
    ui->setupUi(this);
    Scene* scene = new Scene(ui->graphicsView->geometry());
    ui->graphicsView->setScene(scene);
//    ui->graphicsView->scale(0.5, 0.5);
//    scene->setMovable(true);

    Map* map = new Map("tile_map.tmx");

    map->setScene(scene);
    map->addFloor();
    map->addHigher();

    scene->setSceneRect(map->fromLogicToScreen(QPoint(0,0)).x() - scene->sceneRect().width()/2,
                        map->fromLogicToScreen(QPoint(0,0)).y(),
                        scene->sceneRect().width(), scene->sceneRect().height());

    auto game = new Game(map);
}
