#ifndef MAP_H
#define MAP_H

#include "Constants.h"
#include "Object.h"


class GameMap
{
public:
    GameMap() {};
    ~GameMap() {};
    void loadMap(std::string path);

private:
    Map gameMap;
};

#endif