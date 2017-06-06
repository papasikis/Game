#include "gamecreature.h"

GameCreature::GameCreature(QString str, Map *map, const QPoint &node):
    QObject()
{
    graphicsCreature_ = new GraphicsCreature(str, map, node, this);
}

GameCreature::~GameCreature()
{

}

void GameCreature::move(const QList<QPoint> &nodes)
{
    stopFight();
    graphicsCreature_->move(nodes);
}

void GameCreature::attack(GameCreature *enemy, const QList<QPoint> &nodes)
{
    auto connection = new QMetaObject::Connection();
    *connection = connect(graphicsCreature(), &GraphicsCreature::moveStopped,
                                                 [this, connection, enemy](){
        fightWith(enemy);
        hit();
        disconnect(*connection);
        delete connection;
    });

    move(nodes);
}

void GameCreature::hit() {
    if (!inFight_)
        return;

    auto connection = new QMetaObject::Connection();
    *connection = connect(graphicsCreature(), &GraphicsCreature::hitStopped,
                                                 [this, connection](){
        auto timer = new QTimer();
        connect(timer, &QTimer::timeout,
                [this, timer](){
            hit();
            delete timer;
        });
        timer->start(1000/attackSpeed_);
        disconnect(*connection);
        delete connection;
    });

    graphicsCreature_->hit();
    enemy_->getDamage(damage_);
}

void GameCreature::fightWith(GameCreature *enemy)
{
    inFight_ = true;
    enemy_ = enemy;
    connect(enemy_, &GameCreature::die, [this](){
        stopFight();
    });
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
    if (health_ == 0) {
        emit die();
        graphicsCreature()->die();
        connect(graphicsCreature(), &GraphicsCreature::dieStopped, [this](){
            auto timer = new QTimer();
            connect(timer, &QTimer::timeout, [this, timer](){
                delete this;
                delete timer;

            });
            timer->start(3000);
        });
    }
}

QPoint GameCreature::currentNode() const
{
    return graphicsCreature_->currentNode();
}
