#include "game.h"

Game::Game(Map *map): QObject()
{
    setMap(map);
    hero = new Hero("hero_man.xml", map_, QPoint(6, 7));
    map_->scene()->addItem(hero->graphicsCreature());
}

void Game::setMap(Map* map) {
    map_ = map;
    connect(map->scene(), &Scene::mousePressed,
            [this](const QPoint& point){
        qDebug() << map_->fromScreenToNode(point).x() << map_->fromScreenToNode(point).y();
        hero->graphicsCreature()->move(map_->getWayFromTo(hero->graphicsCreature()->currentNode(),
                                                          map_->fromScreenToNode(point)));
    });
}
