#include "Bullet.h"

Bullet::Bullet()
{
    onScreen = false;
}

Bullet::~Bullet() {}

void Bullet::move()
{
    x += VelX;
    y += VelY;

    if (x > (SCREEN_WIDTH + Object::camera.x) || y > (SCREEN_HEIGHT + Object::camera.y) || x < Object::camera.x || y < Object::camera.y)
        onScreen = false;
}

void Bullet::renderBullet()
{
    SDL_Rect renderQuad = {x - Object::camera.x, y - Object::camera.y, rect.w, rect.h};
    SDL_RenderCopy(renderer, texture, NULL, &renderQuad);
}

