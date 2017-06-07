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
    if (enemy == this)
        return;

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
        enemy_->getDamage(damage_);
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
}

void GameCreature::fightWith(GameCreature *enemy)
{
    inFight_ = true;
    enemy_ = enemy;
    connect(enemy_, &GameCreature::dead, [this](){
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
    damage = damage - armor_;
    auto _ = damage;
    QString text;
    while (_ != 0) {
        auto __ = _%10;
        text.push_front('0'+__);
        _/=10;
    }
    text.push_front('-');
    graphicsCreature_->getDamage(text);
    setHealth(-damage);
}

void GameCreature::setHealth(int delta)
{
    health_ += delta;
    if (health_ <= 0) {
        die();
    }
}

void GameCreature::die() {
    emit dead();
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

QPoint GameCreature::currentNode() const
{
    return graphicsCreature_->currentNode();
}
