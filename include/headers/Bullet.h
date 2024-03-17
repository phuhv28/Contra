#ifndef BULLET_H
#define BULLET_H

#include "Constants.h"
#include "Object.h"

class Bullet : public Object
{
public:
    Bullet();
    ~Bullet();
    void move(const SDL_Rect *camera);
    void setVelX(int VelX_) { VelX = VelX_; }
    void setVelY(int VelY_) { VelY = VelY_; }
    void setPos(int x, int y) { this->x = x; this->y = y; }
    void setOnScreen() { onScreen = true; }
    bool getOnScreen() const { return onScreen; }
    void renderBullet(SDL_Renderer *renderer, const SDL_Rect *camera);

private:
    int x, y;
    int VelX, VelY;
    bool onScreen;
};

#endif