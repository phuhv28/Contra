#ifndef ENEMY_H
#define ENEMY_H

#include "Constants.h"
#include "Object.h"
#include "Bullet.h"
#include "Timer.h"

enum class Enemy2Action
{
    AIM_270_DEGREE,
    AIM_90_DEGREE,
    AIM_315_DEGREE,
    AIM_45_DEGREE,
    AIM_225_DEGREE,
    AIM_135_DEGREE
};

enum class Enemy3Action
{
    AIM_90_DEGREE,
    AIM_120_DEGREE,
    AIM_150_DEGREE,
    AIM_180_DEGREE,
    AIM_210_DEGREE,
    AIM_240_DEGREE,
    AIM_270_DEGREE,
    AIM_300_DEGREE,
    AIM_330_DEGREE,
    AIM_0_DEGREE,
    AIM_30_DEGREE,
    AIM_60_DEGREE
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
    std::vector<Bullet*> getBullet() const { return bulletList; }


private:
    Enemy2Action status;
    int numFrame;
    SDL_Rect frameClip[6];
    int frameW;
    int frameH;
    int curFrame;
    int timer;

    std::vector<Bullet*> bulletList;
};

class Enemy3 : public Object
{
public:
    Enemy3();
    ~Enemy3();
    void loadIMG(std::string path);
    void show();
    void action(Map map, int playerX, int playerY);
    void chooseStatus(int playerX, int playerY);
    void createBullet(int playerX, int playerY);
    void handleBullet();
    std::vector<Bullet*> getBullet() const { return bulletList; }


private:
    Enemy3Action status;
    int numFrame;
    SDL_Rect frameClip[12];
    int frameW;
    int frameH;
    int curFrame;
    int timer;

    std::vector<Bullet*> bulletList;
};

#endif