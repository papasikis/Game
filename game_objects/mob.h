#ifndef MOB_H
#define MOB_H
#include "gamecreature.h"

class Mob : public GameCreature
{
public:
    enum Type {
        SkeletonOccultist,
        Zombie
    };
    
    Mob(Type type, Map* map, const QPoint& node);
    
    QString fromTypeToName(Type type);
};

#endif // MOB_H
