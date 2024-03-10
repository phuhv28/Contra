#ifndef PLAYER_H
#define PLAYER_H

#include "Constants.h"
#include "Object.h"


enum class Input
{
    NO_INPUT,
    LEFT,
    RIGHT
};

enum class Direction
{
    LEFT,
    RIGHT
};

class Player : public Object
{
public:
    Player();
    ~Player();

    bool loadIMG(std::string path, SDL_Renderer *renderer);
    void show(SDL_Renderer *renderer);
    void handleInput(SDL_Event e, SDL_Renderer *renderer);
    void action(Map map);

private:
    int VelX, VelY;
    int x, y;
    int frameW, frameH;
    SDL_Rect frameClip[numFrame];
    Input input;
    int curFrame;
    Direction direction;

};

#endif