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
    Map getMap() { return gameMap; }

private:
    Map gameMap;
};

#endif