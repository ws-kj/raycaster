#include "sprite.h"
#include "map.h"

Sprite::Sprite(float x, float y, TileReference* ref) {
    this->posX = x;
    this->posY = y;
    this->reference = ref;
}
