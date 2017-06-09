#ifndef GAME_H
#define GAME_H

#include <QObject>
#include <QDebug>
#include "../game_objects/hero.h"
#include "../game_objects/mob.h"
#include "../game_objects/spawn.h"
#include "map.h"
#include "scene.h"

class Game : public QObject
{
    Q_OBJECT
    Hero* hero;
    Map* map_;

public:
    explicit Game(Map* map);
    void setMap(Map* map);

signals:

public slots:
};

#endif // GAME_H
