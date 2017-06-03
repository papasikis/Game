#include "gamecreature.h"

GameCreature::GameCreature(QString str, Map *map, const QPoint &node)
{
    graphicsCreature_ = new GraphicsCreature(str, map, node);
}
