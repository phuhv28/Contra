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
    void setRect(int x_, int y_);
    SDL_Rect getRect() const { return rect; }
    SDL_Texture *getTexture() const { return texture; }
    virtual void loadIMG(std::string path);
    void render(const SDL_Rect *clip);
    void free();
    void setX(int x_) { x = x_; }
    void setY(int y_) { y = y_; }
    int getX() { return x; }
    int getY() { return y; }

protected:
    SDL_Texture *texture = NULL;
    SDL_Rect rect;
    int x, y;
    int VelX, VelY;
};

#endif