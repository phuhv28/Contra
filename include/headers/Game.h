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
    Game(SDL_Renderer *renderer, SDL_Window *window);
    ~Game();
    void setCamera();
    std::vector<Enemy1 *> createEnemies1();
    std::vector<Enemy2 *> createEnemies2();
    void handleCol();
    void renderGamePlay();
    bool checkCol(const SDL_Rect &a, const SDL_Rect &b);
    void handleEnemy();
    void removeEnemy(int enemyType, int index);
    void renderSplashScreen();
    void renderGameOver();
    void close();
    void run();

private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    Object backGround;
    Player player;
    GameMap map;
    SDL_Event e;
    std::vector<Enemy1 *> enemyList1;
    std::vector<Enemy2 *> enemyList2;
    // The music that will be played
    Mix_Music *gMusic = NULL;
    Mix_Music *gameOver = NULL;

    // The sound effects that will be used
    Mix_Chunk *title = NULL;
    Mix_Chunk *fireSound = NULL;
};

#endif