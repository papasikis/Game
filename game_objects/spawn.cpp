#include "spawn.h"


Spawn::Spawn(const QPoint &centerNode, Map *map, QObject *parent) : QObject(parent),
    map_(map), centerNode_(centerNode)
{
    addMob(Mob::SkeletonOccultist);
    addMob(Mob::SkeletonOccultist);
}

void Spawn::addMob(Mob::Type type)
{
    qsrand(QTime::currentTime().msec());
    auto node = QPoint(qrand()%10 - 5, qrand()%10 - 5) + centerNode_;
    auto mob = new Mob(type, map_, node);
    mobs += mob;
    connect(mob, &Mob::moveStopped, [mob, this](){
//        if (!mob->inFight()){
//            auto randomPoint = QPoint(qrand()%10 - 5, qrand()%10 - 5) + centerNode_;
//            auto timer = new QTimer();
//            connect(timer, &QTimer::timeout, [this, timer, mob, randomPoint](){
//                mob->move(map_->getWayFromTo(mob->currentNode(), randomPoint));
//                delete timer;
//            });
//            timer->start(2000);
//        }
    });
    auto randomPoint = QPoint(qrand()%10 - 5, qrand()%10 - 5) + centerNode_;
    mob->move(map_->getWayFromTo(mob->currentNode(), randomPoint));
}
