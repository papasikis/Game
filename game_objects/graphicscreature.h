#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <QGraphicsObject>
#include <QDomDocument>
#include <QFile>
#include <QDebug>
#include <QPainter>
#include <QPropertyAnimation>
#include <QAbstractAnimation>
#include <QSequentialAnimationGroup>
#include <QParallelAnimationGroup>
#include <QGraphicsScene>
#include "../main/map.h"

//const QString RESOURCES = ":/new/prefix1/isometric_hero/";

class GraphicsCreature : public QGraphicsObject
{
public:
    enum {
        Type = UserType+1
    };

    enum State{
        Run, Stay, Hit, Dead
    };
private:
    Q_OBJECT
    Q_PROPERTY(QPoint sourcePos READ sourcePos WRITE setSourcePos)
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)
    

    QPointF center_ = QPointF(64, 96);
    QPoint sourcePos_ = QPoint(0, 0);
    QSize tileSize_ = QSize(0, 0);
    int runDuration_ = 400;
    QPoint currentNode_;
    Map* map_;
    State state_ = Stay;
    
protected:
    QMap<QString, QPixmap*> source_;
    QVector<QPixmap*> actualThings_;
    QMap<QString,QPropertyAnimation*> animations_;

public:
    explicit GraphicsCreature(const QString& fileName, Map* map, const QPoint& node);
    void setInfo(const QString &fileName);

    QPoint sourcePos() const;
    void setSourcePos(const QPoint &sourcePos);

    void move(QList<QPoint> nodes);
    QPoint currentNode() const {return currentNode_;}

    State state() const {return state_;}
    void changeState(State state);

    // QGraphicsItem interface
public:
    void setPos(const QPointF &point);
    QPointF pos() const;
    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    virtual int type() const override {return Type;}
};

#endif // GAMEOBJECT_H
