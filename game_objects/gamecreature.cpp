#include "gamecreature.h"

GameCreature::GameCreature(QString str, Map *map, const QPoint &node):
    QObject()
{
    graphicsCreature_ = new GraphicsCreature(str, map, node, this);

    connect(graphicsCreature_, &GraphicsCreature::moveStopped, [this](){
        if (inFight_) {
            hit();
        }
        changeState(None);
    });

    connect(graphicsCreature_, &GraphicsCreature::hitStopped, [this](){
        if (inFight_) {
            auto timer = new QTimer();
            connect(timer, &QTimer::timeout, [this, timer](){
               hit();
               delete timer;
            });
            timer->start(1000/attackSpeed_);
        }
    });
}

void GameCreature::move(const QList<QPoint> &nodes)
{
    changeState(Move);
//    if (inFight_)
//        stopFight();
    graphicsCreature_->move(nodes);
}

void GameCreature::attack(GameCreature *enemy, const QList<QPoint> &nodes)
{
    fightWith(enemy);
    move(nodes);
}

void GameCreature::hit() {
    graphicsCreature_->hit();
    enemy_->getDamage(damage_);
}

void GameCreature::fightWith(GameCreature *enemy)
{
    inFight_ = true;
    enemy_ = enemy;
}

void GameCreature::stopFight()
{
    inFight_ = false;
    enemy_ = nullptr;
}

void GameCreature::getDamage(int damage)
{
    graphicsCreature_->getDamage();
    health_ -= damage;
    if (health_ == 0)
        delete this;
}

QPoint GameCreature::currentNode() const
{
    return graphicsCreature_->currentNode();
}
