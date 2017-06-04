#include "map.h"
#include "./../game_objects/graphicscreature.h"

Map::Map(const QString& mapName, Scene *_scene):
    scene_(_scene)
{
    parseTMX(loadTMX(mapName));

}

Map::~Map()
{
    for (auto pixmap: pixmaps) {
        if (pixmap != nullptr)
            delete pixmap;
    }
}

QPoint Map::fromLogicToScreen(const QPoint &logicPoint, int width)
{
    if (width == 0) width = logicWidth;
    return QPoint((logicPoint.x()-logicPoint.y()+width),
                  (logicPoint.x()+logicPoint.y())/2);
}

QPoint Map::fromScreenToLogic(const QPoint &screenPoint)
{
    return QPoint((screenPoint.x()+2*screenPoint.y()-logicWidth)/2,
                  (2*screenPoint.y()-screenPoint.x()+logicWidth)/2);
}

QPoint Map::fromNodeToScreen(const QPoint &node)
{
    return fromLogicToScreen(QPoint((double(node.x())+0.5)*nodeSize, (double(node.y())+0.5)*nodeSize));
}

QPoint Map::fromLogicToNode(const QPoint &logicPoint)
{
    return QPoint(logicPoint.x()/nodeSize, logicPoint.y()/nodeSize);
}

QPoint Map::fromScreenToNode(const QPoint &screenPoint)
{
    return fromLogicToNode(fromScreenToLogic(screenPoint));
}

//void Map::addStaticMapObject(const MapObject *obj)
//{
//    if (obj->barrier != nullptr) {
//        obj->barrier->pos();
//        obj->shape();
//    }
//}

const QVector<QVector<int> > &Map::getFloor() const
{
    return floor;
}

const QVector<QPixmap*> &Map::getPixmaps() const
{
    return pixmaps;
}

const QVector<Map::Tile> &Map::getTiles() const
{
    return tiles;
}

const QVector<QVector<int> > &Map::getHigher() const
{
    return higher;
}

int Map::getLogicWidth() const
{
    return logicWidth;
}

int Map::getTileWidth() const
{
    return /*tileWidth*/64;
}

int Map::getTileHeight() const
{
    return tileHeight;
}

int Map::getNodeSize() const
{
    return nodeSize;
}

void Map::setScene(Scene *value)
{
    scene_ = value;
}

void Map::parseTMX(const QDomDocument &domDoc)
{
    QDomElement map = domDoc.documentElement();
    //TILES
    QList<QDomNode> domTilesets;//for(:)
    for (int i=0; i < map.elementsByTagName("tileset").count(); i++) {
        domTilesets.append(map.elementsByTagName("tileset").at(i));
    }

    tiles.resize(domTilesets.last().toElement().attribute("tilecount").toInt() +
             domTilesets.last().toElement().attribute("firstgid").toInt());
    pixmaps.resize(domTilesets.size());

    for (QDomNode domTileset: domTilesets) {
        auto sourcePixmap = new QPixmap(RESOURCES + domTileset.toElement().elementsByTagName("image").
                             at(0).toElement().attribute("source"));

        pixmaps.push_back(sourcePixmap);
        int width = domTileset.toElement().attribute("tilewidth").toInt();
        int height = domTileset.toElement().attribute("tileheight").toInt();
        int firstId = domTileset.toElement().attribute("firstgid").toInt();
        int count = domTileset.toElement().attribute("tilecount").toInt();

        for (int i=0; i<count; i++) {
            Tile tile;
            tile.width = width;
            tile.height = height;
            tile.source = pixmaps[pixmaps.size()-1];
            tile.x = i*width % tile.source->width();
            tile.y = i*width / tile.source->width() * height;
            tiles[firstId+i] = tile;
        }

        QList<QDomNode> domTiles;//for(:)
        for (int i=0; i<domTileset.toElement().elementsByTagName("tile").count(); i++) {
            domTiles.append(domTileset.toElement().elementsByTagName("tile").at(i));
        }

        for (auto domTile: domTiles) {
            int id = domTile.toElement().attribute("id").toInt();

            QList<QDomNode> domObjects;//for(:)
            for (int i=0; i<domTile.toElement().elementsByTagName("objectgroup").at(0).toElement().
                 elementsByTagName("object").count(); i++) {
                domObjects.append(domTile.toElement().elementsByTagName("objectgroup").at(0).toElement().
                             elementsByTagName("object").at(i));
            }

            for (auto domObject: domObjects) {
                auto domFigure = domObject.toElement().childNodes().at(0);
                tiles[id+firstId].itemName = domObject.toElement().attribute("name");
                QString figureName = domFigure.nodeName();
                QAbstractGraphicsShapeItem* item = 0;
                Tile::Type figureType;
                double x = domObject.toElement().attribute("x").toDouble();
                double y = domObject.toElement().attribute("y").toDouble();

                if (figureName == "ellipse") {
                    figureType = Tile::Type::Ellipse;
                    double width = domObject.toElement().attribute("width").toDouble();
                    double height = domObject.toElement().attribute("height").toDouble();
                    item = new QGraphicsEllipseItem(x, y, width, height);
                }
                else if (figureName == "polygon") {
                    figureType = Tile::Type::Polygon;
                    QVector<QPointF> points;
                    auto _splitted = domFigure.toElement().attribute("points").split(" ");
                    for (QString pointStr: _splitted) {
                        auto pointV = pointStr.split(",");
                        points.append(QPointF(pointV.at(0).toDouble(), pointV.at(1).toDouble()));
                    }
                    item = new QGraphicsPolygonItem(QPolygonF(points));
                    item->setPos(x, y);
                }


                QString type = domObject.toElement().attribute("type");
                if (type == "barrier") {
                    tiles[id+firstId].barrierType = figureType;
                    tiles[id+firstId].barrier = item;
                }
                else if (type == "item") {
                    tiles[id+firstId].itemType = figureType;
                    tiles[id+firstId].item = item;
                }
            }
        }
    }

    //INITIALIZING OF MAP CHARACTERISTICS
    width = map.toElement().attribute("width").toInt();
    height = map.toElement().attribute("height").toInt();
    tileWidth = map.toElement().attribute("tilewidth").toInt();
    tileHeight = map.toElement().attribute("tileheight").toInt();
    //----
//    nodeSize = tileHeight;
    //----
    logicWidth = width*tileHeight;
    logicHeight = height*tileHeight;

    //GETTING MAP ARRAYS
    QDomNodeList list = map.elementsByTagName("layer");

    floor.resize(height);
    for (auto& v: floor) {v.resize(width);}
    higher.resize(height);
    for (auto& v: higher) {v.resize(width);}

    if (list.at(0).toElement().attribute("name") == "Floor") {
        QString str = list.at(0).toElement().elementsByTagName("data").at(0).toElement().text();
        str = str.replace('\n', "");
        QStringList data = str.split(",");
        for (int i=0; i<height*width; i++) {
            floor[i/width][i%width] = data.at(i).toInt();
        }
    }

    if (list.at(1).toElement().attribute("name") == "Higher") {
        QString str = list.at(1).toElement().elementsByTagName("data").at(0).toElement().text();
        str = str.replace('\n', "");
        QStringList data = str.split(",");
        for (int i=0; i<height*width; i++) {
            higher[i/width][i%width] = data.at(i).toInt();
        }
    }

}

QDomDocument Map::loadTMX(const QString &filename)
{
    QDomDocument domDoc;
    QFile file(RESOURCES+filename);
    if(!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Couldn't open " << filename;
    }
    else {
        domDoc.setContent(&file);
    }
    file.close();
    return domDoc;
}

void Map::addFloor()
{
    for (int i=0; i<100; i+=20) {
        for (int j=0; j<100; j+=20) {
            auto floor = new Floor(i, j, 20, 20, this);
            scene_->addItem(floor);
            floor->setPos(fromLogicToScreen(QPoint(i*tileHeight, j*tileHeight)) -
                          QPoint(floor->boundingRect().width()/2, 0));
        }
    }
}

void Map::addHigher(/*QGraphicsScene *scene*/)
{
    for (int i=0; i<height; i++) {
        for (int j=0; j<width; j++) {
            if (higher.at(i).at(j) != 0) {
                auto& tile = tiles.at(higher.at(i).at(j));
                auto higher = new StaticMapObject(tile);
                higher->setPos(fromLogicToScreen(QPoint(j*tileHeight, i*tileHeight)) -
                               QPoint(tile.width/2, tile.height - tileHeight));

                higher->setZValue((i+0.5)*(j+0.25)*2);

                scene_->addItem(higher);
//                auto text = new QGraphicsTextItem(QString::fromStdString(to_string(higher->zValue())),
//                                                  higher);
//                text->setPos(0, 0);
            }
        }
    }
}

QList<QPoint> Map::getWayFromTo(const QPoint &startNode, QPoint endNode)
{
    bool creatureFlag = false;
    auto isEmpty = [this, &endNode, &creatureFlag](QPoint node) {//check if node is empty
        auto points = QVector<QPoint>{
                        QPoint(node.x(), node.y()),
                    QPoint(node.x()+1, node.y()),
                    QPoint(node.x()+1, node.y()+1),
                    QPoint(node.x(), node.y()+1)};
        for (auto& point: points) {
            point *= nodeSize;
            point  = fromLogicToScreen(point);
        }
        
        auto polygon = new QGraphicsPolygonItem(QPolygonF(points));
        scene_->addItem(polygon);

        bool result = true;

        auto collidingItems = polygon->collidingItems();
        for (auto item: collidingItems) {
//            auto rect = item->boundingRect();
//            auto coord = item->pos();
//            auto type = item->type();
//            auto barrierType = Barrier::Type;
//            auto shapeType = QAbstractGraphicsShapeItem::Type;

            if (item->type() == 5 || item->type() == GraphicsCreature::Barrier::Type) {//WTF???
                if (endNode == node) {
                    creatureFlag = true;
                    break;
                }
                result = false;
                break;
            }
        }

//        polygon->setBrush(Qt::black);
        delete polygon;

        return result;
    };

    QList<QPoint> result;
    std::queue<QPoint> q;
    q.push(startNode);

    QVector<QVector<QPoint>> used;
    used.resize(logicWidth/nodeSize);
    for (auto& v: used) {
        v.resize(logicWidth/nodeSize);
        for (auto& f: v)
            f = QPoint(-1, -1);
    }

    used[startNode.x()][startNode.y()] = startNode;

    std::function<void(bool&)> go;
    go = [&result, &used, &go, &isEmpty, &q, &endNode](bool& flag) -> void
    {
        if (q.empty()) {
            return;
        }

        auto currentNode = q.front();
        q.pop();

        if (currentNode == endNode) {
            flag = true;
            return;
        }

        auto directions = QList<QPoint> {
                QPoint(0, -1),
                QPoint(1, 0),
                QPoint(0, 1),
                QPoint(-1, 0),
                QPoint(-1, -1),
                QPoint(1, -1),
                QPoint(1, 1),
                QPoint(-1, 1)
        };

        for (auto direction: directions) {
            auto node = currentNode + direction;

            if (node.x() >= 0 && node.x() < 100 && node.y() >= 0 &&
                    node.y() < 100 && used[node.x()][node.y()] == QPoint(-1, -1))
            {
                used[node.x()][node.y()] = currentNode;
                if (isEmpty(node)) {
                    if (node == endNode) {
                        flag = true;
                        return;
                    }
                    q.push(node);
                }
            }
        }
        go(flag);
    };
    bool flag = false;
    go(flag);//troubles with default parameters within functors

    if (creatureFlag) {
        endNode = used[endNode.x()][endNode.y()];
    }

    if (used[endNode.x()][endNode.y()] != QPoint(-1, -1)) {
        result.push_front(endNode);
        while (result.first() != startNode) {
            result.push_front(used[result.first().x()][result.first().y()]);
        }
    }


    return result;
}

Map::Floor::Floor(int x, int y, int width, int height, Map *map, QGraphicsItem *parent):
    QGraphicsItem(parent), m_x(x), m_y(y), m_width(width), m_height(height), map(map)
{
}

QRectF Map::Floor::boundingRect() const
{
    int test1 = map->getTileWidth(), test2 = map->getTileHeight();
    return QRectF(0, 0, m_width*map->getTileWidth(), m_height*map->getTileHeight());
}

void Map::Floor::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    for (int i=0; i<m_height; i++) {
        for (int j=0; j<m_width; j++) {
            if (map->getFloor().at(i+m_y).at(j+m_x) != 0){
                QPoint screenPoint = map->fromLogicToScreen(//Tile draw point <-width+tileWidth/2
                            QPoint(j*map->getTileHeight(), i * map->getTileHeight()),
                            boundingRect().height()) - QPoint(map->getTileWidth()/2, 0);
                Tile tile = map->getTiles().at(map->getFloor().at(i+m_y).at(j+m_x));
                painter->drawPixmap(screenPoint, *tile.source, QRect(tile.x, tile.y, tile.width,
                                                                     tile.height));
            }
        }
    }
}


StaticMapObject::StaticMapObject(const Map::Tile &tile, QAbstractGraphicsShapeItem *parent):
    MapObject(), tile(&tile)
{
    this->setAcceptHoverEvents(true);

    QAbstractGraphicsShapeItem* barrier;
    if (tile.barrier != nullptr) {
        switch (tile.barrierType) {
        case Tile::Type::Ellipse:
            barrier = new QGraphicsEllipseItem(
                        reinterpret_cast<QGraphicsEllipseItem*>(tile.barrier)->rect());
            break;
        case Tile::Type::Rectangle:
            barrier = new QGraphicsRectItem(
                        reinterpret_cast<QGraphicsRectItem*>(tile.barrier)->rect());
            break;
        case Tile::Type::Polygon:
            barrier = new QGraphicsPolygonItem(
                        reinterpret_cast<QGraphicsPolygonItem*>(tile.barrier)->polygon());
            break;
        default:
            break;
        }

        barrier->setPos(tile.barrier->pos());
        barrier->setParentItem(this);
        barrier->setPen(QPen(Qt::black));
        setBarrier(barrier);
    }

    QAbstractGraphicsShapeItem* item;
    if (tile.item != nullptr) {
        switch (tile.itemType) {
        case Tile::Type::Ellipse:
            item = new QGraphicsEllipseItem(
                        reinterpret_cast<QGraphicsEllipseItem*>(tile.item)->rect());
            break;
        case Tile::Type::Rectangle:
            item = new QGraphicsRectItem(
                        reinterpret_cast<QGraphicsRectItem*>(tile.item)->rect());
            break;
        case Tile::Type::Polygon:
            item = new QGraphicsPolygonItem(
                        reinterpret_cast<QGraphicsPolygonItem*>(tile.item)->polygon());
            break;
        default:
            break;
        }

        item->setPos(tile.item->pos());
        item->setParentItem(this);
    }
}

QRectF StaticMapObject::boundingRect() const
{
    return QRectF(0, 0, tile->width, tile->height);
}

void StaticMapObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawPixmap(QPoint(0,0), *tile->source, QRect(tile->x, tile->y,
                                                          tile->width, tile->height));
}

//void StaticMapObject::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
//{
//    if (item != nullptr)
//        item->setPen(QPen(Qt::red, 3));
//}

//void StaticMapObject::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
//{
//    if (item != nullptr)
//        item->setPen(Qt::NoPen);
//}








