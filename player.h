#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>

class Player {
public:
    double posX, posY;
    double dirX, dirY;
    double planeX, planeY;
    double movementSpeed, rotationSpeed;

    Player(double x, double y, double dX, double dY, double pX, double pY);
    void calculateMovement(sf::Time fps);
};

#endif // PLAYER_H
