#ifndef PLAYER_H
#define PLAYER_H

#include "Constants.h"
#include "Object.h"
#include "Bullet.h"
#include "Timer.h"


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
    AIM_LEFT_WHILE_WALKING,
    AIM_RIGHT_WHILE_WALKING
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
    Player();
    ~Player();

    bool loadIMG(std::string path, SDL_Renderer *renderer);
    void show(SDL_Renderer *renderer);
    void getInput(SDL_Event e, SDL_Renderer *renderer);
    void handleInputQueue(SDL_Event e, SDL_Renderer *renderer);
    void action(Map map);
    int getX() { return x; }
    int getY() { return y; }
    int getFrameW() { return frameW; }
    int getFrameH() { return frameH; }
    void setBullet(std::vector<Bullet*> bulletList) { bullet = bulletList; }
    std::vector<Bullet*> getBullet() const { return bullet; }
    void createBullet(SDL_Renderer *renderer);
    void handleBullet(SDL_Renderer *renderer);
    void setCam(const SDL_Rect &camera);

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
    Timer clock;


    int w[15] = {69, 69, 63, 63, 60, 60, 72, 72, 60, 102, 102, 42, 42, 81, 81};
    int h[15] = {105, 105, 108, 108, 102, 102, 102, 102, 60, 51, 51, 138, 138, 102, 102};
};

#endif