#ifndef HERO_H
#define HERO_H

#include <QGraphicsItem>
#include <QGraphicsItemAnimation>
#include <QPropertyAnimation>
#include <QDomDocument>
#include <QPainter>
#include <QVector>
#include <QTimeLine>
#include <QKeyEvent>
#include "graphicscreature.h"
#include "../main/game.h"

class Hero: public GameCreature
{
//test
public:
    Hero(const QString& infoFile, Map* map, const QPoint& node);

};

#endif // HERO_H
