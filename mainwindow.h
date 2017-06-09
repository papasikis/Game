#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include "menu.h"
#include "main/map.h"
#include "main/scene.h"
#include "main/view.h"
#include "main/game.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void start();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
