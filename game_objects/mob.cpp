#include "mob.h"

Mob::Mob(Type type, Map* map, const QPoint& node):
    GameCreature(fromTypeToName(type), map, node)
{
    connect(this, &Mob::attacked, [this](GameCreature* enemy){
       if (!inFight_)
           attack(enemy);
    });
}

QString Mob::fromTypeToName(Mob::Type type) {
    switch (type) {
    case SkeletonOccultist:
        return "skeleton_occultist";
        break;
    case Zombie:
        return "zombie";
        break;
    default:
        break;
    }
}
