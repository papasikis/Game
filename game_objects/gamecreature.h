#ifndef GAMECREATURE_H
#define GAMECREATURE_H

#include "../main/map.h"
#include "graphicscreature.h"

class GameCreature
{
    GraphicsCreature* graphicsCreature_ = nullptr;
    int orientation_ = 1;

public:
    GameCreature(QString str, Map* map, const QPoint& node);
    GraphicsCreature* graphicsCreature() {return graphicsCreature_;}
};
#endif // GAMECREATURE_H
