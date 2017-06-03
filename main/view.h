#ifndef VIEW_H
#define VIEW_H

#include <QGraphicsView>
#include <QMouseEvent>
#include <iostream>
#include <QApplication>

class View : public QGraphicsView
{

public:
    View(QWidget *parent = 0);
};

#endif // VIEW_H
