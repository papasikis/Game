#include "scene.h"

Scene::Scene(const QRectF &rect):
    QGraphicsScene(rect)
{
    setBackgroundBrush(QBrush(Qt::gray));
//    this->installEventFilter(new MouseEventFilter());
}



void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseMoveEvent(event);
    if (!movable_) return;
    if (mousePress) {
        QPointF difference(mousePress->x() - event->lastScreenPos().x(),
                           mousePress->y() - event->lastScreenPos().y());
        setSceneRect(scenePos.x()+difference.x(), scenePos.y()+difference.y(),
                     sceneRect().width(), sceneRect().height());
//        update(sceneRect());
    }
}


void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit mousePressed(event->lastScenePos().toPoint());
    QGraphicsScene::mousePressEvent(event);
    if (!movable_) return;
    mousePress = new QPointF(event->lastScreenPos());
    scenePos = sceneRect().topLeft();
}

void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{

    QGraphicsScene::mouseReleaseEvent(event);
    if (!movable_) return;
    Q_UNUSED(event)
    delete mousePress;
    mousePress = 0;
}
