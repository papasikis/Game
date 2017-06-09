#include "mainwindow.h"
#include <QApplication>
#include "menu.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    auto mainwindow = new MainWindow();
    mainwindow->show();
    return a.exec();
}
