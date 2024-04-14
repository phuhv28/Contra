#ifndef BULLET_H
#define BULLET_H

#include "Constants.h"
#include "Object.h"

class Bullet : public Object
{
public:
    Bullet();
    ~Bullet();
    void move();
    void setVelX(int VelX_) { VelX = VelX_; }
    void setVelY(int VelY_) { VelY = VelY_; }
    int getX() { return x; }
    int getY() { return y; }
    void setPos(int x, int y) { this->x = x; this->y = y; }
    void setOnScreen() { onScreen = true; }
    bool getOnScreen() const { return onScreen; }
    void renderBullet();

private:
    int x, y;
    int VelX, VelY;
    bool onScreen;
};

#endif