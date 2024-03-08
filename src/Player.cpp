#include "Player.h"

Player::Player()
{
    x = 0;
    y = 0;
    curFrame = 0;
    ValX = 0;
    ValY = 0;
    direction = Direction::RIGHT;
}

Player::~Player()
{
}

bool Player::loadIMG(std::string path, SDL_Renderer *renderer)
{
    bool flag = Object::loadIMG(path, renderer);
    if (!flag)
        std::cout << "Error!";
    else
    {
        frameW = rect.w / 6;
        frameH = rect.h;

        for (int i = 0; i < 6; i++)
        {
            frameClip[i].x = i * frameW;
            frameClip[i].y = 0;
            frameClip[i].w = frameW;
            frameClip[i].h = frameH;
        }
    }

    return flag;
}

void Player::show(SDL_Renderer *renderer)
{
    if (direction == Direction::LEFT)
    {
        loadIMG("res/walking_left.png", renderer);
    }
    else if (direction == Direction::RIGHT)
    {
        loadIMG("res/walking_right.png", renderer);
    }

    if (input == Input::LEFT || input == Input::RIGHT)
    {
        curFrame++;
        if (curFrame >= 50)
            curFrame = 0;
    }
    else
        curFrame = 0;

    rect.x = x;
    rect.y = y;

    SDL_Rect *curClip = &frameClip[curFrame/10];
    SDL_Rect renderQuad = {rect.x, rect.y, frameW, frameH};

    SDL_RenderCopy(renderer, texture, curClip, &renderQuad);
}

void Player::handleInput(SDL_Event e, SDL_Renderer *renderer)
{
    if (e.type == SDL_KEYDOWN)
    {
        switch (e.key.keysym.sym)
        {
        case SDLK_RIGHT:
            direction = Direction::RIGHT;
            input = Input::RIGHT;
            break;
        case SDLK_LEFT:
            direction = Direction::LEFT;
            input = Input::LEFT;
            break;
        default:
            break;
        }
    }
    if (e.type == SDL_KEYUP)
    {
        switch (e.key.keysym.sym)
        {
        case SDLK_RIGHT:
            input = Input::NO_INPUT;
            break;
        case SDLK_LEFT:
            input = Input::NO_INPUT;
            break;
        default:
            break;
        }
    }
    
}
