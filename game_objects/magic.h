#ifndef MAGIC_H
#define MAGIC_H


class Magic
{
public:
    Magic();
    int range;
    int speed;
    int damage;
    enum class Type
    {
        Fire, Ice, Arcane, Dark
    };
};

#endif // MAGIC_H
