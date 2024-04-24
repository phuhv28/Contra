#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <iostream>
#include <SDL.h>
#include <string>
#include <SDL_image.h>
#include <fstream>
#include <chrono>
#include <vector>
#include <algorithm>
#include <cmath>
#include <SDL_mixer.h>
#include <memory>

#define CLOCK_NOW std::chrono::system_clock::now
typedef std::chrono::duration<double, std::milli> ElapsedTime;


const int SCREEN_WIDTH = 768;
const int SCREEN_HEIGHT = 672;
const int FPS = 30;
const double SCREEN_TICKS_PER_FRAME = 1000.0 / FPS;
const int SLOWMOTION_ANIMATION_RATE = 4;
const int PLAYER_BULLET_SPEED = 15;
const int ENEMY_BULLET_SPEED = 5;
const int RELOAD = 5;

const int MAX_NUM_FRAME = 10;
const int PLAYER_SPEED_X = 7;
const int ENEMY_SPEED_X = 7;
const int GRAVITY = 15;
const int INVINCIBLE_TIME = 100;

#define TILE_SIZE  96
#define MAX_MAP_X  104
#define MAX_MAP_Y  7

struct Map
{
    int tile[MAX_MAP_Y][MAX_MAP_X];
};



#endif