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
    SDL_Texture *newTexture = NULL;

    SDL_Surface *loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL)
    {
        std::cout << "Unable to load image! " << path << "SDL_image Error: \n"
                  << IMG_GetError();
    }
    else
    {
        newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        if (newTexture == NULL)
        {
            std::cout << "Unable to create texture from " << path << "SDL Error:\n"
                      << SDL_GetError();
        }
        else
        {
            rect.w = loadedSurface->w;
            rect.h = loadedSurface->h;
        }

        SDL_FreeSurface(loadedSurface);
    }

    texture = newTexture;

    return texture != NULL;
}

void Object::render(SDL_Renderer *renderer, const SDL_Rect *clip)
{
    SDL_RenderCopy(renderer, texture, NULL, &rect);
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
