#ifndef GAMECREATURE_H
#define GAMECREATURE_H

#include "../main/map.h"
#include "graphicscreature.h"
#include <QMetaObject>
#include <QTimer>

class GameCreature: QObject
{
    Q_OBJECT

    GraphicsCreature* graphicsCreature_ = nullptr;
    int orientation_ = 1;
    int damage_ = 1;
    int health_ = 5;
    int attackSpeed_ = 2;
    bool inFight_ = false;
    GameCreature* enemy_ = nullptr;
    enum State {Move, None};
    State state_ = None;

public:
    GameCreature(QString str, Map* map, const QPoint& node);
    GraphicsCreature* graphicsCreature() {return graphicsCreature_;}
    void move(const QList<QPoint>& nodes);
    void attack(GameCreature* enemy, const QList<QPoint> &nodes);
    void getDamage(int damage);
    QPoint currentNode() const;
    State state() const {return state_;}
    void changeState(State state) {state_ = state;}
    void hit();
    void fightWith(GameCreature* enemy);
    void stopFight();
};
#endif // GAMECREATURE_H
