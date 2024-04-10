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
    bool loadIMG(std::string path, SDL_Renderer *renderer);
    void show(SDL_Renderer *renderer, const SDL_Rect &camera);
    void action(Map map, const SDL_Rect &camera);
    void setX( int x_ ) { x = x_; }
    void setY( int y_ ) { y = y_; }
    int getX() { return x; }
    int getY() { return y; }

private:
    // const SDL_Rect *camera;
    // Player *player;

    int VelX, VelY;
    int x, y;
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
    bool loadIMG(std::string path, SDL_Renderer *renderer);
    void show(SDL_Renderer *renderer, const SDL_Rect &camera);
    void action(Map map, const SDL_Rect &camera, int playerX, int playerY, SDL_Renderer *renderer);
    void chooseStatus(int playerX, int playerY);
    void createBullet(int playerX, int playerY, SDL_Renderer *renderer);
    void handleBullet(SDL_Renderer *renderer, const SDL_Rect& camera);
    void setX( int x_ ) { x = x_; }
    void setY( int y_ ) { y = y_; }
    int getX() { return x; }
    int getY() { return y; }

private:
    // const SDL_Rect *camera;
    // Player *player;
    Enemy1Action status;
    int VelX, VelY;
    int x, y;
    int numFrame;
    SDL_Rect frameClip[6];
    int frameW;
    int frameH;
    int curFrame;
    Timer timer;

    std::vector<Bullet*> bulletList;
};

#endif