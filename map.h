#ifndef MAP_H
#define MAP_H

#include <vector>
#include <map>
#include <SFML/Graphics.hpp>

const int MAP_WIDTH = 24;
const int MAP_HEIGHT = 24;

extern int worldMap[MAP_WIDTH][MAP_HEIGHT];

extern std::map<char, sf::Image> mapTextures;

class Map {
public:
    std::vector<std::vector<char>> worldMap;
    int width;
    int height;
    Map(std::string path);
};

#endif // MAP_H
