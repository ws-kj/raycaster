#ifndef RAYCASTER_H
#define RAYCASTER_H

#include <vector>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "player.h"
#include "config.h"
#include "map.h"

class Raycaster {
public:
    sf::Uint32 buffer[WINDOW_WIDTH][WINDOW_HEIGHT];
    double zBuffer[WINDOW_WIDTH];

    std::vector<int> spriteOrder;
    std::vector<double> spriteDistance;

    Raycaster();
    void cast(Player player, sf::RenderWindow* window);
    void sortSprites();
};

#endif // RAYCASTER_H
