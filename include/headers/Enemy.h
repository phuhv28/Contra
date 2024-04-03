#ifndef ENEMY_H
#define ENEMY_H

#include "Constants.h"
#include "Object.h"

class Enemy : public Object
{
public:
    Enemy();
    ~Enemy();
    bool loadIMG(std::string path, SDL_Renderer *renderer);
    void show(SDL_Renderer *renderer, const SDL_Rect &camera);
    void action(Map map, const SDL_Rect &camera);
    void setX( int x_ ) { x = x_; }
    void setY( int y_ ) { y = y_; }

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

#endif