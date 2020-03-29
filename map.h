#ifndef MAP_H
#define MAP_H

#include <vector>
#include <map>
#include <SFML/Graphics.hpp>

#include "sprite.h"

const int MAP_WIDTH = 24;
const int MAP_HEIGHT = 24;

extern int worldMap[MAP_WIDTH][MAP_HEIGHT];
extern sf::Image mapTextures[8];
extern std::vector<Sprite> sprites;

void generateTextures();

#endif // MAP_H
