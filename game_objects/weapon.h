#ifndef WEAPON_H
#define WEAPON_H
#include "item.h"
//#include "hero.h"

class Weapon : public Item
{
public:
    Weapon();
   // Hero* hero = nullptr;
    int damage;
    int range;
    int speed;
    enum class Type
    {
        Sword
    };
/*protected:
    void use(Weapon* weapon, Inventory* inventory);
    void take(Weapon* weapon);
    void throwAway(Weapon* weapon);*/
};

#endif // WEAPON_H
