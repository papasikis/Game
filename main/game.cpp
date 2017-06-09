#include "game.h"

Game::Game(Map *map): QObject()
{
    setMap(map);

    hero = new Hero(map_, QPoint(6, 7));
    auto mob = new Mob(Mob::SkeletonOccultist, map_, QPoint(10, 8));

//    auto spawn = new Spawn(QPoint(20,20), map);
    connect(hero->graphicsCreature(), &GraphicsCreature::posChanged, [this](){
        auto sRect = map_->scene()->sceneRect();
        auto gCreature = hero->graphicsCreature();
        map_->scene()->setSceneRect(gCreature->pos().x()-sRect.width()/2,
                                    gCreature->pos().y()-sRect.height()/2,
                                    sRect.width(), sRect.height());
        map_->scene()->update();
    });
}

void Game::setMap(Map* map) {
    map_ = map;
    connect(map->scene(), &Scene::mousePressed,
            [this](const QPoint& point){
        auto item = map_->scene()->itemAt(point, QTransform());
        if (item != nullptr) {
            switch (item->type()) {
            case GraphicsCreature::Body::Type: {

                auto body = dynamic_cast<GraphicsCreature::Body*>(item);
                auto parent = body->parent();
                auto enemy = reinterpret_cast<GameCreature*>(parent->parent());

                hero->attack(enemy);
                break;
            }
            default:
                hero->move(map_->getWayFromTo(hero->graphicsCreature()->currentNode(),
                                                                  map_->fromScreenToNode(point)));
                break;
            }

        }
    });
}
