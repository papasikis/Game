#ifndef GAMECREATURE_H
#define GAMECREATURE_H

#include "../main/map.h"
#include "graphicscreature.h"
#include <QMetaObject>
#include <QTimer>

class GameCreature: public QObject
{
    Q_OBJECT

    GraphicsCreature* graphicsCreature_ = nullptr;
    int orientation_ = 1;
    int damage_ = 5;
    int health_ = 20;
    int armor_ = 1;
    float attackSpeed_ = 1.5;
    GameCreature* enemy_ = nullptr;
    enum State {Move, None};
    State state_ = None;

protected:
    bool inFight_ = false;
public:
    GameCreature(QString str, Map* map, const QPoint& node);
    ~GameCreature();
    GraphicsCreature* graphicsCreature() {return graphicsCreature_;}
    void move(const QList<QPoint>& nodes);
    void attack(GameCreature* enemy);
    void getDamage(int damage, GameCreature* enemy);
    QPoint currentNode() const;
    State state() const {return state_;}
    void changeState(State state) {state_ = state;}
    void hit();
    void fightWith(GameCreature* enemy);
    void stopFight();

    void setHealth(int delta);
    void die();

signals:
    void dead() const;
    void attacked(GameCreature* enemy);
};
#endif // GAMECREATURE_H
