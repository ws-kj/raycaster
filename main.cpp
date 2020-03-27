#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <stdlib.h>
#include <math.h>

#include "map.h"
#include "config.h"
#include "player.h"
#include "raycaster.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Raycaster");

    window.setFramerateLimit(60);
    sf::Clock clock = sf::Clock();
    sf::Time fps;

    Player player = Player(22, 12, -1, 0, 0, 0.66);
    Raycaster caster = Raycaster();
    Map map = Map("/home/will/raycaster/maps/test.rcmap");

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
            window.close();

            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
            window.close();
        }

        window.clear();

        sf::RectangleShape upper = sf::RectangleShape(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT / 2));
        upper.setFillColor(sf::Color(204, 255, 255));
        upper.setPosition(0, 0);
        window.draw(upper);

        sf::RectangleShape lower = sf::RectangleShape(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT / 2));
        lower.setFillColor(sf::Color(168, 168, 168));
        lower.setPosition(0, WINDOW_HEIGHT / 2);
        window.draw(lower);


        caster.cast(player, &window, map);

        fps = clock.getElapsedTime();
        clock.restart();

        player.calculateMovement(fps);

        window.display();
        window.clear();
    }

    return EXIT_SUCCESS;
}
