#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <stdlib.h>
#include <math.h>
#include <vector>

#include "map.h"
#include "config.h"
#include "player.h"
#include "raycaster.h"
#include "sprite.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Raycaster");

    window.setFramerateLimit(60);
    sf::Clock clock = sf::Clock();
    sf::Time fps;

    Player player = Player(22, 12, -1, 0, 0, 0.66);
    Raycaster caster = Raycaster();

    Sprite barrel = Sprite(22.0f, 21.0f, 6);
    sprites.push_back(barrel);

    generateTextures();

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
            window.close();

            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
            window.close();
        }

        window.clear();

        caster.cast(player, &window);

        fps = clock.getElapsedTime();
        clock.restart();

        player.calculateMovement(fps);

        window.display();
        window.clear();
    }

    return EXIT_SUCCESS;
}
