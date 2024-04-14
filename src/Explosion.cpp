#include "Explosion.h"

Explosion::Explosion()
{
    loadIMG("res/img/explosion.png");
    for (int i = 0; i < 3; i++)
    {
        frameClip[i].x = i * 90;
        frameClip[i].y = 0;
        frameClip[i].w = 90;
        frameClip[i].h = 90;
    }
    x = 0;
    y = 0;
    timer = 0;
    aCurFrame = 0;
    curFrame = 0;
    done = false;
}

Explosion::~Explosion()
{
}

void Explosion::action()
{
}

void Explosion::render()
{
    if (!done)
    {
        aCurFrame++;
        curFrame = aCurFrame / SLOWMOTION_ANIMATION_RATE;

        SDL_Rect *curClip = &frameClip[curFrame];

        if (x < Object::camera.x)
            x = Object::camera.x;
        if (x > Object::camera.x + SCREEN_WIDTH)
            x = Object::camera.x + SCREEN_WIDTH;
        if (y < Object::camera.y)
            y = Object::camera.y;
        if (y > Object::camera.y + SCREEN_HEIGHT)
            y = Object::camera.y + SCREEN_HEIGHT;

        rect.x = x - Object::camera.x;
        rect.y = y - Object::camera.y;

        SDL_Rect renderQuad = {rect.x, rect.y, 90, 90};

        SDL_RenderCopy(renderer, texture, curClip, &renderQuad);
    }
}