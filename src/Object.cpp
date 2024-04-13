#include "Object.h"

Object::Object()
{
    rect.x = 0;
    rect.y = 0;
    rect.w = 0;
    rect.h = 0;
}

Object::~Object()
{
}

void Object::setRect(int x, int y)
{
    rect.x = x;
    rect.y = y;
}

bool Object::loadIMG(std::string path, SDL_Renderer *renderer)
{
    texture = IMG_LoadTexture(renderer, path.c_str());
    SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
    return texture != NULL;
}

void Object::render(SDL_Renderer *renderer, const SDL_Rect *clip)
{
    SDL_Rect renderQuad = rect;

    if (clip != NULL)
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    SDL_RenderCopy(renderer, texture, clip, &renderQuad);
}

void Object::free()
{
    if (texture != NULL)
    {
        SDL_DestroyTexture(texture);
        texture = NULL;
        rect.w = 0;
        rect.h = 0;
    }
}
