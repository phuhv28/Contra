#ifndef PLAYER_H
#define PLAYER_H

#include "Constants.h"
#include "Object.h"
#include "Bullet.h"


enum class Input
{
    NO_INPUT,
    LEFT,
    RIGHT,
    JUMP
};

struct Direction
{
    bool left, right, up, down;
    Direction() : left(false), right(false), up(false), down(false) {};
};

class Player : public Object
{
public:
    Player(const SDL_Rect *camera);
    ~Player();

    bool loadIMG(std::string path, SDL_Renderer *renderer);
    void show(SDL_Renderer *renderer);
    void handleInput(SDL_Event e, SDL_Renderer *renderer);
    void action(Map map);
    int getX() { return x; }
    int getY() { return y; }
    int getFrameW() { return frameW;}
    int getFrameH() { return frameH;}
    void setBullet(std::vector<Bullet*> bulletList) { bullet = bulletList; }
    std::vector<Bullet*> getBullet() const { return bullet; }
    void createBullet(SDL_Renderer *renderer);
    void handleBullet(SDL_Renderer *renderer);

private:
    const SDL_Rect *camera;
    
    int VelX, VelY;
    int x, y;
    bool isFalling;
    int numFrame;
    int frameW, frameH;
    SDL_Rect frameClip[MAX_NUM_FRAME];
    int curFrame;
    Direction direction;
    bool onGround;
    int aCurFrame;
    std::vector<Input> inputQueue;
    std::vector<Bullet*> bullet;
};

#endif