#include <math.h>
#include <SFML/Graphics.hpp>

#include "raycaster.h"
#include "player.h"
#include "config.h"
#include "map.h"

Raycaster::Raycaster() {}

void Raycaster::cast(Player player, sf::RenderWindow* window) {
    //floor/ceiling
    for(int y = 0; y < WINDOW_HEIGHT; y++) {
      float rayDirX0 = player.dirX - player.planeX;
      float rayDirY0 = player.dirY - player.planeY;
      float rayDirX1 = player.dirX + player.planeX;
      float rayDirY1 = player.dirY + player.planeY;

      int p = y - WINDOW_HEIGHT / 2;
      float posZ = 0.5 * WINDOW_WIDTH;
      float rowDistance = posZ / p;

      float floorStepX = rowDistance * (rayDirX1 - rayDirX0) / WINDOW_WIDTH;
      float floorStepY = rowDistance * (rayDirY1 - rayDirY0) / WINDOW_WIDTH;

      float floorX = player.posX + rowDistance * rayDirX0;
      float floorY = player.posY + rowDistance * rayDirY0;

      for(int x = 0; x < WINDOW_WIDTH; x++) {
           int cellX = (int)(floorX);
           int cellY = (int)(floorY);

           int tx = (int)(TEX_WIDTH * (floorX - cellX)) & (TEX_WIDTH - 1);
           int ty = (int)(TEX_HEIGHT * (floorY - cellY)) & (TEX_HEIGHT - 1);

           floorX += floorStepX;
           floorY += floorStepY;

           //floor
           sf::Color tcolor = mapTextures[1].getPixel(tx, ty);
           sf::Uint32 color = tcolor.toInteger();
           buffer[y][x] = color;

            //ceiling
           tcolor = mapTextures[0].getPixel(tx, ty);

           color = tcolor.toInteger();
           buffer[WINDOW_HEIGHT - y - 1][x] = color;

      }
    }

    //walls
    for(int x = 0; x < WINDOW_WIDTH; x++) { 
        double cameraX = 2 * x / double(WINDOW_WIDTH) - 1; //x-coordinate in camera space
        double rayPosX = player.posX;
        double rayPosY = player.posY;
        double rayDirX = player.dirX + player.planeX * cameraX;
        double rayDirY = player.dirY + player.planeY * cameraX;

        //which box of the map we're in
        int mapX = int(rayPosX);
        int mapY = int(rayPosY);

        //length of ray from current position to next x or y-side
        double sideDistX;
        double sideDistY;

        //length of ray from one x or y-side to next x or y-side
        double deltaDistX = sqrt(1 + (rayDirY * rayDirY) / (rayDirX * rayDirX));
        double deltaDistY = sqrt(1 + (rayDirX * rayDirX) / (rayDirY * rayDirY));
        double perpWallDist;

        //what direction to step in x or y-direction (either +1 or -1)
        int stepX;
        int stepY;

        int hit = 0; //was there a wall hit?
        int side; //was a NS or a EW wall hit?

        //calculate step and initial sideDist
        if (rayDirX < 0) {
            stepX = -1;
            sideDistX = (rayPosX - mapX) * deltaDistX;
        }
        else {
            stepX = 1;
            sideDistX = (mapX + 1.0 - rayPosX) * deltaDistX;
        }
        if (rayDirY < 0) {
            stepY = -1;
            sideDistY = (rayPosY - mapY) * deltaDistY;
        }
        else {
            stepY = 1;
            sideDistY = (mapY + 1.0 - rayPosY) * deltaDistY;
        }

        //perform DDA
        while (hit == 0) {
            //jump to next map square, OR in x-direction, OR in y-direction
            if (sideDistX < sideDistY) {
                sideDistX += deltaDistX;
                mapX += stepX;
                side = 0;
            }
            else {
                sideDistY += deltaDistY;
                mapY += stepY;
                side = 1;
            }
            //Check if ray has hit a wall
            if (worldMap[mapX][mapY] > 0) hit = 1;
        }

        if (side == 0)
            perpWallDist = fabs((mapX - rayPosX + (1 - stepX) / 2) / rayDirX);
        else
            perpWallDist = fabs((mapY - rayPosY + (1 - stepY) / 2) / rayDirY);

        //Calculate height of line to draw on screen
        int lineHeight = abs(int(WINDOW_HEIGHT / perpWallDist));

        //calculate lowest and highest pixel to fill in current stripe
        int drawStart = -lineHeight / 2 + WINDOW_HEIGHT / 2;
        if(drawStart < 0)drawStart = 0;
        int drawEnd = lineHeight / 2 + WINDOW_HEIGHT / 2;
        if(drawEnd >= WINDOW_HEIGHT) drawEnd = WINDOW_HEIGHT - 1;

        int texNum = worldMap[mapX][mapY] - 1;
        double wallX;
        if (side == 0) wallX = player.posY + perpWallDist * rayDirY;
        else           wallX = player.posX + perpWallDist * rayDirX;
        wallX -= floor((wallX));

        int texX = int(wallX * double(TEX_WIDTH));
        if(side == 0 && rayDirX > 0) texX = TEX_WIDTH - texX - 1;
        if(side == 1 && rayDirY < 0) texX = TEX_WIDTH - texX - 1;

        double step = 1.0 * TEX_HEIGHT / lineHeight;
        double texPos = (drawStart - WINDOW_HEIGHT / 2 + lineHeight / 2) * step;
        for(int y = drawStart; y<drawEnd; y++) {
            int texY = (int)texPos & (TEX_HEIGHT - 1);
            texPos += step;

            sf::Color tcolor = mapTextures[texNum].getPixel(texX, texY);
            if(side == 1) {
                tcolor.r /= 2;
                tcolor.g /= 2;
                tcolor.b /= 2;
            }
            sf::Uint32 color = tcolor.toInteger();

            buffer[y][x] = color;
        }

    }

    sf::Uint8* pixels = new sf::Uint8[WINDOW_WIDTH*WINDOW_HEIGHT*4];
    sf::Texture texture;
    texture.create(WINDOW_WIDTH, WINDOW_HEIGHT);
    sf::Sprite sprite(texture);

    int i = 0;
    for(int y = 0; y < WINDOW_HEIGHT; y++) {
        for(int x = 0; x < WINDOW_WIDTH; x++) {
            pixels[i]   = sf::Color(buffer[y][x]).r;// & 0xFF;
            pixels[i+1] = sf::Color(buffer[y][x]).g;// & 0xFF;
            pixels[i+2] = sf::Color(buffer[y][x]).b;// & 0xFF;
            pixels[i+3] = sf::Color(buffer[y][x]).a;// & 0xFF;
            i += 4;
        }
    }
    texture.update(pixels);
    window->draw(sprite);

    //bhkprintf("%d\n", texture.isSrgb());


    for(int y = 0; y < WINDOW_HEIGHT; y++) {
        for(int x = 0; x < WINDOW_WIDTH; x++) {
            buffer[y][x] = 0;
        }
    }
}

