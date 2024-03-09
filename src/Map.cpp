#include "Map.h"


void GameMap::loadMap(std::string path)
{
    std::fstream map;
    map.open(path);

    for (int i = 0; i < MAX_MAP_Y; i++)
    {
        for (int j = 0; j < MAX_MAP_X; j++)
        {
            map >> gameMap.tile[i][j];
        }
    }

    map.close();
}
