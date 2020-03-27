#ifndef RAYCASTER_H
#define RAYCASTER_H

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "player.h"
#include "config.h"
#include "map.h"

class Raycaster {
public:
    Raycaster();

    void cast(Player player, sf::RenderWindow* window, Map map);

    sf::Image textures[8];
    sf::Uint32 buffer[WINDOW_WIDTH][WINDOW_HEIGHT];
};

#endif // RAYCASTER_H
