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
    RIGHT,
    UP
};

class Player : public Object
{
public:
    Player();
    ~Player();

    bool loadIMG(std::string path, SDL_Renderer *renderer);
    void show(SDL_Renderer *renderer, const SDL_Rect *camera);
    void handleInput(SDL_Event e, SDL_Renderer *renderer);
    void action(Map map);
    int getX() { return x; }
    int getY() { return y; }
    int getFrameW() { return frameW;}
    int getFrameH() { return frameH;}

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