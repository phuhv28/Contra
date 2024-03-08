#ifndef OBJECT_H
#define OBJECT_H

#include "Constants.h"

class Object
{
public:
    Object();
    ~Object();
    void setRect(int x, int y);
    SDL_Rect getRect() const { return rect; }
    SDL_Texture *getTexture() const { return texture; }
    virtual bool loadIMG(std::string path, SDL_Renderer *renderer);
    void render(SDL_Renderer *renderer, const SDL_Rect *clip = NULL);
    void free();

protected:
    SDL_Texture *texture = NULL;
    SDL_Rect rect;
};

#endif