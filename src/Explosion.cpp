#include "Explosion.h"

Explosion::Explosion()
{
    for (int i = 0; i < 3; i++)
    {
        frameClip[i].x = i * 90;
        frameClip[i].y = 0;
        frameClip[i].w = 90;
        frameClip[i].h = 90;
    }
    timer = 0;
    aCurFrame = 0;
    curFrame = 0;
    done = false;
}

Explosion::~Explosion()
{
}

void Explosion::render()
{
    aCurFrame++;
    curFrame = aCurFrame / SLOWMOTION_ANIMATION_RATE;
    if (curFrame >= 3)
    {
        aCurFrame = 0;
    }
    if (!done)
    {
        // if (texture == NULL)
        //     std::cout << "Error";
        SDL_Rect *curClip = &frameClip[curFrame];

        rect.x = x - Object::camera.x;
        rect.y = y - Object::camera.y;

        SDL_Rect renderQuad = {rect.x, rect.y, 90, 90};

        SDL_RenderCopy(renderer, texture, curClip, &renderQuad);
        // std::cout << curFrame << " ";
    }
}