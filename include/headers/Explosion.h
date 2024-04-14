#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "Constants.h"
#include "Object.h"

class Explosion : public Object
{
private:
    int x, y;
    SDL_Rect frameClip[3];
    int timer;
    int aCurFrame;
    int curFrame;
    bool done;


public:
    Explosion();
    ~Explosion();
    void setPos(int x, int y) { this->x = x; this->y = y; }
    void action();
    void render();
};



#endif