#ifndef BRIDGE_H
#define BRIDGE_H

#include "Constants.h"
#include "Explosion.h"
#include "Map.h"

class Bridge : public Object
{
public:
    Bridge();
    ~Bridge();
    void show();
    void action(int playerX, GameMap &gameMap);

private:
    bool isExploding;
    int curFrame;
    int aCurFrame;
    Explosion explosion;
    SDL_Rect frameClip[5];

};


#endif