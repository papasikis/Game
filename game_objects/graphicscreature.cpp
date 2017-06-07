#include "graphicscreature.h"
//----------------------------------------------------------
QList<QDomNode> toQList(const QDomNodeList& list) {
    QList<QDomNode> result;
    for (int i=0; i<list.size(); i++)
        result.push_back(list.at(i));
    return result;
}
//----------------------------------------------------------

QPoint GraphicsCreature::sourcePos() const
{
    return sourcePos_;
}

void GraphicsCreature::setSourcePos(const QPoint &sourcePos)
{
    sourcePos_ = sourcePos;
    if (scene() != nullptr)
        scene()->update(mapRectToScene(boundingRect()));
}

void GraphicsCreature::move(QList<QPoint> nodes)
{
    if (nodes.empty() || nodes[0] != currentNode_)
        return;

    auto seq = new QSequentialAnimationGroup();
    auto prevNode = currentNode_;

    for (auto node: nodes) {
        auto dir = node - prevNode;
        int direction;
        if (dir == QPoint(0, 1))      {
            direction = 7;
        } else if (dir == QPoint(1, 1)) {
            direction = 6;
        } else if (dir == QPoint(1, 0)) {
            direction = 5;
        } else if (dir == QPoint(1, -1)) {
            direction = 4;
        } else if (dir == QPoint(0, -1)) {
            direction = 3;
        } else if (dir == QPoint(-1, -1)) {
            direction = 2;
        } else if (dir == QPoint(-1, 0)){
            direction = 1;
        } else if (dir == QPoint(-1, 1)) {
            direction = 0;
        } else
            continue;

        auto animation = new QParallelAnimationGroup();

        auto changeSourcePos = new QPropertyAnimation(this, "sourcePos");
        changeSourcePos->setStartValue(QPoint(4, direction));
        changeSourcePos->setEndValue(QPoint(11, direction));

        auto changePos = new QPropertyAnimation(this, "pos");
        changePos->setStartValue(map_->fromNodeToScreen(prevNode));
        changePos->setEndValue(map_->fromNodeToScreen(node));

        changePos->setDuration(runDuration_);
        changeSourcePos->setDuration(runDuration_);

        prevNode = node;

        if (direction == 0 || direction == 2 || direction == 4 || direction == 6) {//for diagonal nodes
            changePos->setDuration(runDuration_ * 1.4);
            changeSourcePos->setDuration(runDuration_ * 1.4);
        }

        animation->addAnimation(changeSourcePos);
        animation->addAnimation(changePos);

        seq->addAnimation(animation);
    }

    changeState(Run);
    connect(seq, &QSequentialAnimationGroup::finished,
            [this](){
        changeState(Stay);
        emit moveStopped();
    });
    if (seq->animationCount() == 0) {
        seq->finished();
    }
    else
        seq->start(QAbstractAnimation::DeleteWhenStopped);

}

void GraphicsCreature::hit()
{
    auto animation = new QPropertyAnimation(this, "sourcePos");
    animation->setStartValue(QPoint(12, sourcePos().y()));
    animation->setEndValue(QPoint(15, sourcePos().y()));
    connect(animation, &QPropertyAnimation::finished,
            [this](){
       changeState(Stay);
       emit hitStopped();
    });

    changeState(Hit);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void GraphicsCreature::getDamage(const QString& text)
{
    auto animation = new QPropertyAnimation(this, "sourcePos");
    animation->setStartValue(QPoint(16, sourcePos().y()));
    animation->setEndValue(QPoint(17, sourcePos().y()));
    connect(animation, &QPropertyAnimation::finished,
            [this](){
       changeState(Stay);
       emit getDamageStopped();
    });
    changeState(Damaged);
    animation->start(QAbstractAnimation::DeleteWhenStopped);

    auto textItem = new QGraphicsTextItem(text, this);
    textItem->setDefaultTextColor(Qt::red);
    textItem->setPos(50, 30);

    auto textAnimation = new QPropertyAnimation(textItem, "pos");
    textAnimation->setEndValue(QPoint(50, 0));
    textAnimation->setDuration(1500);
    connect(textAnimation, &QPropertyAnimation::finished, [textItem](){
       delete textItem;
    });
    textAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void GraphicsCreature::die()
{
    auto animation = new QPropertyAnimation(this, "sourcePos");
    animation->setStartValue(QPoint(18, sourcePos().y()));
    animation->setEndValue(QPoint(23, sourcePos().y()));
    animation->setDuration(1000);
    connect(animation, &QPropertyAnimation::finished,
            [this](){
       emit dieStopped();
    });
    changeState(Dead);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
    delete body_;
    delete barrier_;
}



void GraphicsCreature::changeState(GraphicsCreature::State state)
{
    state_ = state;
    if (state_ == Stay) {
        auto stayAnimation = new QPropertyAnimation(this, "sourcePos");
        stayAnimation->setStartValue(QPoint(0,sourcePos().y()));
        stayAnimation->setKeyValueAt(0.5, QPoint(3,sourcePos().y()));
        stayAnimation->setEndValue(QPoint(0,sourcePos().y()));
        stayAnimation->setDuration(1000);

        connect(stayAnimation, &QPropertyAnimation::finished,
                [this](){
            if(state_ == Stay) {
                changeState(Stay);
            }
        });

        stayAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }

}


void GraphicsCreature::setPos(const QPointF &point)
{   
    auto _pos = point-center_;
    QGraphicsItem::setPos(_pos);
    currentNode_ = map_->fromScreenToNode(point.toPoint());
    setZValue(currentNode().x()*currentNode().y());

    auto bodyPos = mapToScene(55, 55).toPoint();
    body_->setPos(bodyPos);

    emit posChanged();
}

QPointF GraphicsCreature::pos() const
{
    return QGraphicsItem::pos() + center_;
}

GraphicsCreature::GraphicsCreature(const QString& fileName, Map *map, const QPoint &node, QObject *parent) :
    QObject(parent) ,map_(map)
{
    setInfo(fileName);

    barrier_ = new Barrier(QRect(center_.x()-15, center_.y()-7, 30, 15), this);
    body_ = new Body(QRect(0, 0, 20, 40), this);

    map_->scene()->addItem(body_);
    map_->scene()->addItem(this);
    setPos(map->fromNodeToScreen(node));

    changeState(Stay);
}

void GraphicsCreature::setInfo(const QString &fileName)
{
    QDomDocument domDoc;
    QFile file(":/new/prefix1/resources/"+fileName+"/"+fileName+".xml");
    if(!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Couldn't open " << fileName;
    }
    else {
        domDoc.setContent(&file);
    }
    file.close();

    QDomElement creature = domDoc.elementsByTagName("creature").at(0).toElement();
    auto itemGroups = toQList(creature.elementsByTagName("itemgroup"));

    tileSize_ = QSize(creature.attribute("tilewidth").toInt(),
                     creature.attribute("tileheight").toInt());
    center_ = QPoint(creature.attribute("centerX").toInt(),
                    creature.attribute("centerY").toInt());

    for (auto itemGroup: itemGroups) {
        auto images = toQList(itemGroup.toElement().elementsByTagName("image"));
        for (auto image: images) {
            auto pixmap = new QPixmap(":/new/prefix1/resources/"+fileName+"/"+
                                      image.toElement().attribute("source"));
            if (pixmap != nullptr)
                source_[image.toElement().attribute("name")] = pixmap;
            else {
                qDebug() << "Image "+image.toElement().attribute("name")+
                            " does not exist!";
                exit(1);
            }
        }
    }

    auto domAnimations = toQList(creature.elementsByTagName("animation"));

    for (auto domAnimation: domAnimations) {
//        auto animation = new QPropertyAnimation(this, "frame");

//        auto start = domAnimation.toElement().attribute("firstid").toInt();
//        auto end = start + domAnimation.toElement().attribute("count").toInt();

//        animation->setStartValue(start);
//        animation->setEndValue(end);

//        animations_[domAnimation.toElement().attribute("type")] = animation;
    }

}

QRectF GraphicsCreature::boundingRect() const
{
    return QRectF(0, 0, tileSize_.width(), tileSize_.height());
}

void GraphicsCreature::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                       QWidget *widget)
{
    for (auto pixmap: source_) {
        painter->drawPixmap(QPoint(0, 0), *pixmap,
                            QRect(sourcePos_.x()*tileSize_.width(),
                                  sourcePos_.y()*tileSize_.height(),
                                  boundingRect().width(), boundingRect().height()));
    }

//    for (auto item: this->collidingItems()) {
//        if (item->type() == MapObject::Type)
//        {
//            qDebug() << "hui";
//        }
//    }
}


