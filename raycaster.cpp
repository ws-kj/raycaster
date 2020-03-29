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
        zBuffer[x] = perpWallDist;
    }
    //draw sprites
    for(int i = 0; i < sprites.size(); i++) {
        spriteOrder.push_back(i);
        spriteDistance.push_back(
            (player.posX - sprites[i].posX)
            * (player.posX - sprites[i].posX)
            + (player.posY - sprites[i].posY)
            * (player.posY - sprites[i].posY));

    }
    this->sortSprites();

    for(int i = 0; i < sprites.size(); i++) {
        double spriteX = sprites[spriteOrder[i]].posX - player.posX;
        double spriteY = sprites[spriteOrder[i]].posY - player.posY;

        double invDet = 1.0 / (player.planeX * player.dirY - player.dirY * player.planeY);

        double transformX = invDet * (player.dirY * spriteX - player.dirX * spriteY);
        double transformY = invDet * (-player.planeY * spriteX + player.planeX * spriteY);

        int spriteScreenX = int((WINDOW_WIDTH / 2) * (1 + transformX / transformY));

        int spriteHeight = abs(int(WINDOW_HEIGHT / (transformY)));

        int drawStartY = -spriteHeight / 2 + WINDOW_HEIGHT / 2;
        if(drawStartY < 0) drawStartY = 0;
        int drawEndY = spriteHeight / 2 + WINDOW_HEIGHT / 2;
        if(drawEndY >= WINDOW_HEIGHT) drawEndY = WINDOW_HEIGHT - 1;

        int spriteWidth = abs(int(WINDOW_HEIGHT / (transformY)));
        int drawStartX = -spriteWidth / 2 + spriteScreenX;
        if(drawStartX < 0) drawStartX = 0;
        int drawEndX = spriteWidth / 2 + spriteScreenX;
        if(drawEndX >= WINDOW_WIDTH) drawEndX = WINDOW_WIDTH - 1;

        for(int stripe = drawStartX; stripe < drawEndX; stripe++) {
            int texX = int(256 * (stripe - (-spriteWidth / 2 + spriteScreenX)) * TEX_WIDTH / spriteWidth) / 256;

            if(transformY > 0 && stripe > 0 && stripe < WINDOW_WIDTH && transformY < zBuffer[stripe])
            for(int y = drawStartY; y < drawEndY; y++) {
                int d  = (y) * 256 - WINDOW_HEIGHT * 128 + spriteHeight * 128;
                int texY = ((d * TEX_HEIGHT) / spriteHeight) / 256;

                sf::Color tcolor = mapTextures[sprites[spriteOrder[i]].texture].getPixel(texX, texY);
                sf::Uint32 color = tcolor.toInteger();
                buffer[y][stripe] = color;
            }
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

void Raycaster::sortSprites() {
    std::vector<std::pair<double, int>> spr(sprites.size());
      for(int i = 0; i < sprites.size(); i++) {
        spr[i].first = spriteDistance[i];
        spr[i].second = spriteOrder[i];
      }
      std::sort(spr.begin(), spr.end());
      // restore in reverse order to go from farthest to nearest
      for(int i = 0; i < sprites.size(); i++) {
        spriteDistance[i] = spr[sprites.size() - i - 1].first;
        spriteOrder[i] = spr[sprites.size() - i - 1].second;
      }
}
