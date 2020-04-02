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

sf::View getLetterboxView(sf::View view, int windowWidth, int windowHeight);

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Raycaster");
    window.setSize(sf::Vector2u(TRUE_WINDOW_WIDTH, TRUE_WINDOW_HEIGHT));
    window.setFramerateLimit(60);
    sf::Clock clock = sf::Clock();
    sf::Time fps;

    Player player = Player(22, 12, -1, 0, 0, 0.66);
    Raycaster caster = Raycaster();

    generateTiles();
    loadLevelMap(lowerMap);
    loadLevelMap(midMap);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

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

sf::View getLetterboxView(sf::View view, int windowWidth, int windowHeight) {

    // Compares the aspect ratio of the window to the aspect ratio of the view,
    // and sets the view's viewport accordingly in order to archieve a letterbox effect.
    // A new view (with wa new viewport set) is returned.

    float windowRatio = windowWidth / (float) windowHeight;
    float viewRatio = view.getSize().x / (float) view.getSize().y;
    float sizeX = 1;
    float sizeY = 1;
    float posX = 0;
    float posY = 0;

    bool horizontalSpacing = true;
    if (windowRatio < viewRatio)
        horizontalSpacing = false;

    // If horizontalSpacing is true, the black bars will appear on the left and right side.
    // Otherwise, the black bars will appear on the top and bottom.

    if (horizontalSpacing) {
        sizeX = viewRatio / windowRatio;
        posX = (1 - sizeX) / 2.f;
    }

    else {
        sizeY = windowRatio / viewRatio;
        posY = (1 - sizeY) / 2.f;
    }

    view.setViewport( sf::FloatRect(posX, posY, sizeX, sizeY) );

    return view;
}
