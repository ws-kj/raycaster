#ifndef MAP_H
#define MAP_H

#include <vector>
#include <map>
#include <SFML/Graphics.hpp>

const int MAP_WIDTH = 24;
const int MAP_HEIGHT = 24;

enum textureChars {
    wood = 'w',
    greystone = 'g',
    purplestone = 'p',
    redbrick = 'r',
    mossy = 'm',
    colorstone = 'c',
    barrel = 'B',
    pillar = 'P',

};

struct TileReference {
    std::string name;
    int symbol;
    sf::Image texture;
    bool collidable;
    bool sprite;
};

extern std::vector<std::vector<int>> worldMap;
extern std::vector<TileReference> tileReferences;

void generateTiles();
void createTile(std::string name, int symbol, bool collidable=true, bool sprite=false);
TileReference* getTileReference(int symbol);

#endif // MAP_H
