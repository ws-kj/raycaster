#include "map.h"
#include "config.h"

std::vector<std::vector<int>> worldMap
{
        {'w','w','w','w','w','w','w','w','w','w','w','w','w','w','w','w','w','w','w','w','w','w','w','w'},
        {'w',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','w'},
        {'w',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','w'},
        {'w',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','w'},
        {'w',' ',' ',' ',' ',' ','g','g','g','g','g',' ',' ',' ',' ','r',' ','r',' ','r',' ',' ',' ','w'},
        {'w',' ',' ',' ',' ',' ','g','P','P','P','g',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','w'},
        {'w',' ',' ',' ',' ',' ','g',' ',' ',' ','g',' ',' ',' ',' ','r',' ',' ',' ','r',' ',' ',' ','w'},
        {'w',' ',' ',' ',' ',' ','g',' ','L',' ','g',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','w'},
        {'w',' ',' ',' ',' ',' ','g','g',' ','g','g',' ',' ',' ',' ','r',' ','r',' ','r',' ',' ',' ','w'},
        {'w',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','w'},
        {'w',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','w'},
        {'w',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','w'},
        {'w',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','B',' ',' ',' ','B',' ',' ',' ','w'},
        {'w',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','w'},
        {'w',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','B',' ',' ',' ',' ',' ','w'},
        {'w',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','w'},
        {'w','m','m','m','m','m','m','m','m',' ',' ',' ',' ',' ',' ','B',' ',' ',' ','B',' ',' ',' ','w'},
        {'w','m',' ','m',' ',' ',' ',' ','m',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','w'},
        {'w','m',' ',' ',' ',' ','e',' ','m',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','w'},
        {'w','m',' ','m',' ',' ',' ',' ','m',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','w'},
        {'w','m',' ','m','m','m','m','m','m',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','w'},
        {'w','m',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','w'},
        {'w','m','m','m','m','m','m','m','m',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','w'},
        {'w','w','w','w','w','w','w','w','w','w','w','w','w','w','w','w','w','w','w','w','w','w','w','w'}
};

std::vector<TileReference> tileReferences;
std::vector<Sprite> sprites;
void generateTiles() {
    TileReference empty;
    empty.name = " ";
    empty.symbol = ' ';
    empty.collidable = false;
    tileReferences.push_back(empty);

    createTile("wood", 'w');
    createTile("mossy", 'm');
    createTile("eagle", 'e');
    createTile("greystone", 'g');
    createTile("redbrick", 'r');

    createTile("barrel", 'B', true, true);
    createTile("pillar", 'P', true, true);
    createTile("greenlight", 'L', false, true);
}

int loadMap(std::vector<std::vector<int>> map) {
    for(int i = 0; i < map.size(); i++) {
        for(int j = 0; j < map[i].size(); i++) {
            TileReference* ref;
            if(getReference(map[i][j]) != NULL) {
                ref = getReference(map[i][j]);
            } else {
                ref = getReference(' ');
            }

            if(ref->sprite) {
                Sprite sprite = Sprite((float)i, (float)j, ref);
                sprites.push_back(sprite);
            }

        }
    }

    return 0;
}

void createTile(std::string name, int symbol, bool collidable, bool sprite) {
    TileReference ref;
    ref.name = name;
    ref.symbol = symbol;
    ref.collidable = collidable;
    ref.sprite = sprite;

    sf::Image image;
    image.loadFromFile("/home/will/raycaster/pics/" + name + ".png");
    ref.texture = image;

    tileReferences.push_back(ref);
}

TileReference* getReference(int symbol) {
    for(int i = 0; i < tileReferences.size(); i++) {
        if(tileReferences[i].symbol == symbol) {
            return &tileReferences[i];
        }
    }
    return NULL;
}

