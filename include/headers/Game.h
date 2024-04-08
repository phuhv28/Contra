#ifndef GAME_H
#define GAME_H

#include "Constants.h"
#include "Object.h"
#include "Player.h"
#include "Enemy.h"
#include "Map.h"

class Game
{
public:
    Game();
    ~Game();
    bool init();
    void setCamera();
    std::vector<Enemy *> createEnemies();
    void handleCol();
    void renderGamePlay();
    bool checkCol(const SDL_Rect& a, const SDL_Rect& b);
    void close();
    void run();

private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    Object backGround;
    SDL_Rect camera;
    Player player;
    GameMap map;
    SDL_Event e;
    std::vector<Enemy *> enemyList;
};

#endif