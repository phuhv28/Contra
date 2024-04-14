#ifndef OBJECT_H
#define OBJECT_H

#include "Constants.h"

class Object
{
public:
    static SDL_Rect camera;
    static SDL_Renderer *renderer;

    Object();
    ~Object();
    void setRect(int x, int y);
    SDL_Rect getRect() const { return rect; }
    SDL_Texture *getTexture() const { return texture; }
    virtual bool loadIMG(std::string path);
    void render(const SDL_Rect *clip);
    void free();

protected:
    SDL_Texture *texture = NULL;
    SDL_Rect rect;
};



#endif