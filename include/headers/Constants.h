#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <iostream>
#include <SDL.h>
#include <string>
#include <SDL_image.h>
#include <fstream>


const int SCREEN_WIDTH = 768;
const int SCREEN_HEIGHT = 672;

const int numFrame = 7;

const int SPEED_X = 5;
const int GRAVITY = 10;

#define TILE_SIZE  96
#define MAX_MAP_X  104
#define MAX_MAP_Y  7

struct Map
{
    int x, y;
    int tileX, tileY;

    int tile[MAX_MAP_Y][MAX_MAP_X];
};



#endif