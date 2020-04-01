#ifndef SPRITE_H
#define SPRITE_H

#include "map.h"

class Sprite
{
public:
    Sprite(float x, float y, TileReference* ref);
    float posX, posY;
    int texture;
    TileReference* reference;
};

#endif // SPRITE_H
