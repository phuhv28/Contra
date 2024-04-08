#include "Bullet.h"

Bullet::Bullet()
{
    x = 0;
    y = 0;
    VelX = 0;
    VelY = 0;
    onScreen = false;
}

Bullet::~Bullet() {}

void Bullet::move(const SDL_Rect *camera)
{
    x += VelX;
    y += VelY;

    if (x > (SCREEN_WIDTH + camera->x) || y > (SCREEN_HEIGHT + camera->y) || x < camera->x || y < camera->y)
        onScreen = false;
}

void Bullet::renderBullet(SDL_Renderer *renderer, const SDL_Rect *camera)
{
    SDL_Rect renderQuad = {x - camera->x, y - camera->y, rect.w, rect.h};
    SDL_RenderCopy(renderer, texture, NULL, &renderQuad);
}

