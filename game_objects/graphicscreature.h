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

class GraphicsCreature :public QObject, public QGraphicsItem
{

public:
    class Barrier: public QGraphicsEllipseItem
    {
    public:
        Barrier(const QRect& rect, QGraphicsItem* parent):
        QGraphicsEllipseItem(rect, parent)
        {
            setPen(QPen(Qt::black));
        }
        enum {Type = UserType-1234};

    public:
        virtual int type() const override
        {
            return Type;
        }
    };

    class Body: public QObject, public QGraphicsRectItem
    {
    public:
        Body(const QRect& rect, QObject* parent):
            QObject(parent),
            QGraphicsRectItem(rect)
        {
            setPen(QPen(Qt::red));
            setZValue(1000000000000000);
        }
        enum {Type = UserType-4321};

    public:
        virtual int type() const override
        {
            return Type;
        }
    };

    enum {
        Type = UserType-20000
    };

    enum State{
        Run, Stay, Hit, Dead, Damaged
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

    QMap<QString, QPixmap*> source_;
    QVector<QPixmap*> actualThings_;
    QMap<QString,QPropertyAnimation*> animations_;

    Barrier* barrier_ = nullptr;
    Body* body_ = nullptr;

public:
    explicit GraphicsCreature(const QString& fileName, Map* map, const QPoint& node,
                              QObject* parent = nullptr);
    void setInfo(const QString &fileName);

    QPoint sourcePos() const;
    void setSourcePos(const QPoint &sourcePos);

    void move(QList<QPoint> nodes);
    void hit();
    void getDamage();
    void die();

    QPoint currentNode() const {return currentNode_;}

    State state() const {return state_;}
    void changeState(State state);

    Body* body() {return body_;}

signals:
    void moveStopped() const;
    void hitStopped() const;
    void getDamageStopped() const;
    void dieStopped() const;
    void posChanged() const;

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
