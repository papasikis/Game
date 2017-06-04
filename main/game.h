#ifndef GAME_H
#define GAME_H

#include <QObject>
#include "../game_objects/gamecreature.h"
#include "../game_objects/hero.h"
#include "map.h"
#include "scene.h"

class Game : public QObject
{
    Q_OBJECT
    GameCreature* hero;
    Map* map_;

public:
    explicit Game(Map* map);
    void setMap(Map* map);
    void addOnScene(GameCreature* creature);

signals:

public slots:
};

#endif // GAME_H
