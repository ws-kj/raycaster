#include <math.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "player.h"
#include "map.h"

Player::Player(double x, double y, double dX, double dY, double pX, double pY) {
    this->posX = x;
    this->posY = y;
    this->dirX = dX;
    this->dirY = dY;
    this->planeX = pX;
    this->planeY = pY;
    this->movementSpeed = 5.0;
    this->rotationSpeed = 3.0;
}

void Player::calculateMovement(sf::Time fps) {
    double moveSpeed = fps.asSeconds() * this->movementSpeed; //the constant value is in squares/second
    double rotSpeed = fps.asSeconds() * this->rotationSpeed; //the constant value is in radians/second

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        //both camera direction and camera plane must be rotated
        double oldDirX = this->dirX;
        this->dirX = this->dirX * cos(rotSpeed) - this->dirY * sin(rotSpeed);
        this->dirY = oldDirX * sin(rotSpeed) + this->dirY * cos(rotSpeed);
        double oldPlaneX = this->planeX;
        this->planeX = this->planeX * cos(rotSpeed) - this->planeY * sin(rotSpeed);
        this->planeY = oldPlaneX * sin(rotSpeed) + this->planeY * cos(rotSpeed);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        //both camera direction and camera plane must be rotated
        double oldDirX = this->dirX;
        this->dirX = this->dirX * cos(-rotSpeed) - this->dirY * sin(-rotSpeed);
        this->dirY = oldDirX * sin(-rotSpeed) + this->dirY * cos(-rotSpeed);
        double oldPlaneX = this->planeX;
        this->planeX = this->planeX * cos(-rotSpeed) - this->planeY * sin(-rotSpeed);
        this->planeY = oldPlaneX * sin(-rotSpeed) + this->planeY * cos(-rotSpeed);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        if(!getTileReference(worldMap[int(this->posX + this->dirX * moveSpeed)][int(this->posY)])->collidable) this->posX += this->dirX * moveSpeed;
        if(!getTileReference(worldMap[int(this->posX)][int(this->posY + this->dirY * moveSpeed)])->collidable) this->posY += this->dirY * moveSpeed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        if(!getTileReference(worldMap[int(this->posX - this->dirX * moveSpeed)][int(this->posY)])->collidable) this->posX -= this->dirX * moveSpeed;
        if(!getTileReference(worldMap[int(this->posX)][int(this->posY - this->dirY * moveSpeed)])->collidable) this->posY -= this->dirY * moveSpeed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        if(!getTileReference(worldMap[int(this->posX + this->planeX * moveSpeed)][int(this->posY)])->collidable) this->posX += this->planeX * moveSpeed;
        if(!getTileReference(worldMap[int(this->posX)][int(this->posY + this->planeY * moveSpeed)])->collidable) this->posY += this->planeY * moveSpeed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        if(!getTileReference(worldMap[int(this->posX - this->planeX * moveSpeed)][int(this->posY)])->collidable) this->posX -= this->planeX * moveSpeed;
        if(!getTileReference(worldMap[int(this->posX)][int(this->posY - this->planeY * moveSpeed)])->collidable) this->posY -= this->planeY * moveSpeed;
    }
}
