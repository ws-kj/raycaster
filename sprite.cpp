#include "sprite.h"
#include "map.h"

std::vector<Sprite> sprites;

Sprite::Sprite(float x, float y, TileReference* ref) {
    this->posX = x;
    this->posY = y;
    this->reference = ref;
}
