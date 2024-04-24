#include "Object.h"

SDL_Rect Object::camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
SDL_Renderer *Object::renderer = nullptr;

Object::Object()
{
    rect = {0, 0, 0, 0};
    x = 0;
    y = 0;
    VelX = 0;
    VelY = 0;
}

Object::~Object()
{
    free();
}

void Object::setRect(int x_, int y_)
{
    rect.x = x_;
    rect.y = y_;
}

void Object::loadIMG(std::string path)
{
    texture = IMG_LoadTexture(renderer, path.c_str());
    SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
    if (texture == NULL)
        std::cout << SDL_GetError() << std::endl;
}

void Object::render(const SDL_Rect *clip)
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
