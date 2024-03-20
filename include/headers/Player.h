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
    X,
    Z,
    DOWN,
    UP
};

enum class Action
{
    AIM_DOWN_RIGHT_WHILE_WALKING,
    AIM_DOWN_LEFT_WHILE_WALKING,
    AIM_UP_LEFT_WHILE_WALKING,
    AIM_UP_RIGHT_WHILE_WALKING,
    WALKING_LEFT,
    WALKING_RIGHT,
    STANDING_LEFT,
    STANDING_RIGHT,
    JUMPING,
    LAYING_DOWN_LEFT,
    LAYING_DOWN_RIGHT,
    AIM_UP_LEFT,
    AIM_UP_RIGHT,

};

struct Direction
{
    bool left, right, up, down;
    Direction() : left(false), right(false), up(false), down(false) {};
};

struct Status
{
    bool isFalling;
    bool onGround;
    bool isFiring;
    Action action;
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
    int frameW, frameH;
    int numFrame;
    SDL_Rect frameClip[MAX_NUM_FRAME];
    int curFrame;
    Direction direction;
    int aCurFrame;
    std::vector<Input> inputQueue;
    std::vector<Bullet*> bullet;
    Status status;
    bool lay;
    int dx = 0;
};

#endif