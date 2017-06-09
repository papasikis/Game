#ifndef SPAWN_H
#define SPAWN_H

#include <QObject>
#include <QDateTime>
#include "mob.h"

class Spawn : public QObject
{
    Q_OBJECT
    Map* map_ = nullptr;
    QPoint centerNode_;
    QSet<Mob*> mobs;
public:
    explicit Spawn(const QPoint& centerNode, Map* map, QObject *parent = 0);
    void addMob(Mob::Type type);
signals:

public slots:
};

#endif // SPAWN_H
