#ifndef MAP_H
#define MAP_H

#include <QObject>
#include <QtXml/qdom.h>
#include <QtXml/QDomDocument>
#include <QGraphicsItem>
#include <QFile>
#include <QPixmap>
#include <QGraphicsScene>
#include <QDebug>
#include <QPainter>
#include <QGraphicsEllipseItem>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsPolygonItem>
#include <functional>
#include <queue>
#include "scene.h"


using namespace std;

const int NODE = 16;
const QString RESOURCES = ":/new/prefix1/resources/";

class Map: public QObject
{
    Q_OBJECT
public:
    class Floor: public QGraphicsItem {
        Map* map;
        int m_width, m_height, m_x, m_y;

    public:
        Floor(int x, int y, int width, int height, Map* map, QGraphicsItem *parent = 0);
        // QGraphicsItem interface
        virtual QRectF boundingRect() const override;
        virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                           QWidget *widget) override;
    };
    
    struct Tile {
        enum class Type {
            Rectangle, Polygon, Ellipse
        };
        const QPixmap* source = nullptr;
        int x = 0, y = 0, width = 0, height = 0;
        Type barrierType = Type::Polygon;
        QAbstractGraphicsShapeItem * barrier = nullptr;
        Type itemType = Type::Polygon;
        QAbstractGraphicsShapeItem* item = nullptr;
        QString itemName = "";
    };

    Map(const QString &mapName, Scene* _scene = nullptr);
    ~Map();
    QPoint fromLogicToScreen(const QPoint& logicPoint, int width = 0);
    QPoint fromScreenToLogic(const QPoint& screenPoint);
    QPoint fromNodeToScreen(const QPoint& node);
    QPoint fromLogicToNode(const QPoint& logicPoint);
    QPoint fromScreenToNode(const QPoint& screenPoint);

    void addFloor(/*QGraphicsScene* scene*/);
    void addHigher();

    QList<QPoint> getWayFromTo(const QPoint& startNode, const QPoint& endNode);

    void parseTMX(const QDomDocument &domDoc);
    QDomDocument loadTMX(const QString &filename);
    const QVector<QVector<int> >& getFloor() const;
    const QVector<QPixmap *> &getPixmaps() const;
    const QVector<Tile>& getTiles() const;
    const QVector<QVector<int> >& getHigher() const;
    int getLogicWidth() const;
    int getTileWidth() const;
    int getTileHeight() const;
    int getNodeSize() const;

    void setScene(Scene* _scene);
    Scene* scene() {return scene_;}

private:

    //tileHeight use in program like as logicTileSize(side)
    int logicWidth, width, height, logicHeight, tileWidth, tileHeight;
    int nodeSize;

    QVector<Tile> tiles;
    QVector<QPixmap*> pixmaps;
    Scene* scene_;

    QVector<QVector<int>> floor;
    QVector<QVector<int>> higher;
    QVector<QVector<bool>> nodes;
};

class Barrier: public QAbstractGraphicsShapeItem {
public:
    Barrier(QAbstractGraphicsShapeItem* item):
    QAbstractGraphicsShapeItem(item)
    {}
    enum {Type = UserType+1};
    int type() const override {return Type;}
};

class MapObject: public QAbstractGraphicsShapeItem {

private:
    QAbstractGraphicsShapeItem* item_;
    Barrier* barrier_;
public:
    MapObject(QAbstractGraphicsShapeItem* item = nullptr,
              QAbstractGraphicsShapeItem* barrier = nullptr):
        item_(item), barrier_(dynamic_cast<Barrier*>(barrier)){}

    enum {Type = UserType+1};
    virtual int type() const override {return Type;}

    Barrier *barrier() const
    {
        return barrier_;
    }
    QAbstractGraphicsShapeItem *item() const
    {
        return item_;
    }
    void setItem(QAbstractGraphicsShapeItem *item)
    {
        item_ = item;
    }
    void setBarrier(QAbstractGraphicsShapeItem *barrier)
    {
        barrier_ = dynamic_cast<Barrier*>(barrier);
    }
};

class StaticMapObject: public QObject, public MapObject {
        Q_OBJECT
    using Tile = Map::Tile;
public:
    enum {Type = UserType+1};
    enum class GameType {Chest, Monster};
    GameType getGameType() const {return gameType;}

    StaticMapObject(const Tile& tile, QAbstractGraphicsShapeItem* parent = 0);

    // QGraphicsItem interface
    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    virtual int type() const override {return Type;}
//    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
//    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

//signals:
//    void hoverEnter(GameType type);
//    void hoverLeave(GameType type);

private:
    const Tile* tile;
    GameType gameType;

};

#endif // MAP_H
