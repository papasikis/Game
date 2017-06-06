#include "game.h"

Game::Game(Map *map): QObject()
{
    setMap(map);
    hero = new Hero("hero_man.xml", map_, QPoint(6, 7));
    auto mob = new Hero("hero_man.xml", map_, QPoint(4, 7));
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

                hero->attack(enemy, map_->getWayFromTo(hero->currentNode(), enemy->currentNode()));
                break;
            }
            default:
//                qDebug() << item->zValue();
                hero->move(map_->getWayFromTo(hero->graphicsCreature()->currentNode(),
                                                                  map_->fromScreenToNode(point)));
                break;
            }

        }
    });
}

void Game::addOnScene(GameCreature *creature)
{
    map_->scene()->addItem(creature->graphicsCreature());
}
