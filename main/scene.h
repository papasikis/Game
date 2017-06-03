#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QKeyEvent>
#include <QGraphicsItem>

class Scene : public QGraphicsScene
{
    Q_OBJECT

    bool movable_ = false;
    QPointF* mousePress = 0;
    QPointF scenePos;

//    class MouseEventFilter: public QObject
//    {
//    public:
//        virtual bool eventFilter(QObject *watched, QEvent *event) override;
//    };

public:
    Scene(const QRectF& rect);
    void setMovable(bool flag = true){movable_=flag;}

signals:
    void mousePressed(const QPoint& point) const;

    // QGraphicsScene interface
protected:
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // SCENE_H
