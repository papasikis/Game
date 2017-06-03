#ifndef ARMOR_H
#define ARMOR_H
#include "hero.h"

class Armor : public Item
{
    Hero* hero = nullptr;
public:
    Armor();
    int resistance;
};

#endif // ARMOR_H
