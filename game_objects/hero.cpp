#include "hero.h"

Hero::Hero(Map *map, const QPoint &node):
    GameCreature("hero_man", map, node)
{
    graphicsCreature()->setFlag(QGraphicsItem::ItemIsMovable);
    graphicsCreature()->setFlag(QGraphicsItem::ItemIsFocusable);

//    if (graphicsCreature()->body() != nullptr) {
//        delete graphicsCreature()->body();
//    }
}


//void Hero::keyPressEvent(QKeyEvent *event)
//{
//    switch (event->key()) {
//    case Qt::Key_Up:
//        moveBy(0, -10);
//        break;
//    case Qt::Key_Down:
//        moveBy(0, 10);
//        break;
//    case Qt::Key_Left:
//        moveBy(-10, 0);
//        break;
//    case Qt::Key_Right:
//        moveBy(10, 0);
//        break;
//    default:
//        break;
//    }
//}
