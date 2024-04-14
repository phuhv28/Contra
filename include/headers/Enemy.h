#ifndef ENEMY_H
#define ENEMY_H

#include "Constants.h"
#include "Object.h"
#include "Bullet.h"
#include "Timer.h"

enum class Enemy1Action
{
    AIM_LEFT,
    AIM_RIGHT,
    AIM_UP_LEFT,
    AIM_UP_RIGHT,
    AIM_DOWN_LEFT,
    AIM_DOWN_RIGHT
};

class Enemy1 : public Object
{
public:
    Enemy1();
    ~Enemy1();
    void loadIMG(std::string path);
    void show();
    void action(Map map);


private:

    int numFrame;
    SDL_Rect frameClip[6];
    int frameW;
    int frameH;
    int curFrame;
    int aCurFrame;
    bool onGround;
};

class Enemy2 : public Object
{
public:
    Enemy2();
    ~Enemy2();
    void loadIMG(std::string path);
    void show();
    void action(Map map, int playerX, int playerY);
    void chooseStatus(int playerX, int playerY);
    void createBullet(int playerX, int playerY);
    void handleBullet();

private:
    Enemy1Action status;
    int numFrame;
    SDL_Rect frameClip[6];
    int frameW;
    int frameH;
    int curFrame;
    int timer;

    std::vector<Bullet*> bulletList;
};

#endif