#include "Player.h"

Player::Player()
{
    x = 100;
    y = 0;
    curFrame = 0;
    VelX = 0;
    VelY = GRAVITY;
    direction = Direction::RIGHT;
}

Player::~Player()
{
}

bool Player::loadIMG(std::string path, SDL_Renderer *renderer)
{
    free();

    bool flag = Object::loadIMG(path, renderer);
    if (!flag)
        std::cout << "Error!";
    else
    {
        for (int i = 0; i < 6; i++)
        {
            frameClip[i].x = i * 60;
            frameClip[i].y = 0;
            frameClip[i].w = 60;
            frameClip[i].h = 105;
        }

        frameClip[6].x = 6 * 60;
        frameClip[6].y = 0;
        frameClip[6].w = 69;
        frameClip[6].h = 105;
    }

    return flag;
}

void Player::show(SDL_Renderer *renderer, const SDL_Rect *camera)
{
    if (direction == Direction::LEFT)
    {
        loadIMG("res/left.png", renderer);
    }
    else if (direction == Direction::RIGHT)
    {
        loadIMG("res/right.png", renderer);
    }

    if (input == Input::LEFT || input == Input::RIGHT)
    {
        curFrame++;
        if (curFrame >= 5)
            curFrame = 0;
    }
    else
        curFrame = 6;

    SDL_Rect *curClip = &frameClip[curFrame];

    rect.x = x - camera->x;
    rect.y = y - camera->y;
    frameH = curClip->h;
    frameW = curClip->w;

    SDL_Rect renderQuad = {rect.x, rect.y, frameW, frameH};

    SDL_RenderCopy(renderer, texture, curClip, &renderQuad);
}

void Player::handleInput(SDL_Event e, SDL_Renderer *renderer)
{
    if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
    {
        switch (e.key.keysym.sym)
        {
        case SDLK_RIGHT:
            direction = Direction::RIGHT;
            input = Input::RIGHT;
            VelX += SPEED_X;
            break;
        case SDLK_LEFT:
            direction = Direction::LEFT;
            input = Input::LEFT;
            VelX -= SPEED_X;
            break;
        }
    }
    else if (e.type == SDL_KEYUP && e.key.repeat == 0)
    {
        switch (e.key.keysym.sym)
        {
        case SDLK_RIGHT:
            VelX -= SPEED_X;
            if (VelX == 0)
                input = Input::NO_INPUT;
            break;
        case SDLK_LEFT:
            VelX += SPEED_X;
            if (VelX == 0)
                input = Input::NO_INPUT;
            break;
        }

        if (VelX > 0) direction = Direction::RIGHT;
        if (VelX < 0) direction = Direction::LEFT;
    }
}

void Player::action(Map map)
{
    if (map.tile[(y + VelY) / TILE_SIZE + 1][(x + VelX) / TILE_SIZE] == 1 ||
        map.tile[(y + VelY) / TILE_SIZE + 1][(x + VelX + frameClip[curFrame].w) / TILE_SIZE] == 1)
    {
        VelY = 0;
        y += VelY;
        // std::cout << y << " ";
    }

    if (VelY == 0 &&
        (map.tile[(y + VelY) / TILE_SIZE + 1][(x + VelX) / TILE_SIZE] != 1 && map.tile[(y + VelY) / TILE_SIZE + 1][(x + VelX + frameClip[curFrame].w) / TILE_SIZE] != 1))
    {
        VelY = 1;
        // std::cout << y << " ";
    }

    x += VelX;
    y += VelY;
    // std::cout << y << "\n";

    if (x < 0 || x > MAX_MAP_X * TILE_SIZE)
        x = 0;
    if (y < 0 || y > MAX_MAP_Y * TILE_SIZE)
        y = 0;
}
