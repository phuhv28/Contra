#include "Player.h"

Player::Player()
{
    x = 100;
    y = 0;
    curFrame = 0;
    VelX = 0;
    VelY = GRAVITY;
    direction.right = true;
    onGround = false;
    aCurFrame = 0;
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

    if (path == "res/jump.png")
    {
        for (int i = 0; i < 6; i++)
        {
            frameClip[i].x = i * 60;
            frameClip[i].y = 0;
            frameClip[i].w = 60;
            frameClip[i].h = 60;
        }
    }
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
    if (onGround == false)
    {
        loadIMG("res/jump.png", renderer);
    }
    else
    {
        // std::cout << direction.left << " " << direction.right << std::endl;
        if (direction.left == true)
        {
            loadIMG("res/left.png", renderer);
        }
        else if (direction.right == true)
        {
            loadIMG("res/right.png", renderer);
        }
    }

    if ((int)inputQueue.size() != 0 || onGround == false)
    {
        aCurFrame++;
        if (aCurFrame >= 5 * 4)
            aCurFrame = 0;
    }
    else
        aCurFrame = 6 * 4;

    curFrame = aCurFrame / 4;

    SDL_Rect *curClip = &frameClip[curFrame];

    if (x < camera->x)
        x = camera->x;
    if (x > camera->x + SCREEN_WIDTH)
        x = camera->x + SCREEN_WIDTH;
    if (y < camera->y)
        y = camera->y;
    if (y > camera->y + SCREEN_HEIGHT)
        y = camera->y + SCREEN_HEIGHT;

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
            inputQueue.push_back(Input::RIGHT);
            break;
        case SDLK_LEFT:
            inputQueue.push_back(Input::LEFT);
            break;
        case SDLK_x:
            if (onGround == true)
            {
                direction.up = true;
                VelY = -17;
                onGround = false;
            }
        }
    }
    else if (e.type == SDL_KEYUP && e.key.repeat == 0)
    {
        switch (e.key.keysym.sym)
        {
        case SDLK_RIGHT:
            for (int i = 0; i < (int)inputQueue.size(); i++)
            {
                if (inputQueue[i] == Input::RIGHT)
                    inputQueue.erase(inputQueue.begin() + i);
            }
            break;
        case SDLK_LEFT:
            for (int i = 0; i < (int)inputQueue.size(); i++)
            {
                if (inputQueue[i] == Input::LEFT)
                    inputQueue.erase(inputQueue.begin() + i);
            }
            break;
        }
    }

    if ((int)inputQueue.size() == 1)
    {
        if (inputQueue[0] == Input::RIGHT)
        {
            VelX = SPEED_X;
            direction.right = true;
            direction.left = false;
        }
        else
        {
            VelX = -SPEED_X;
            direction.left = true;
            direction.right = false;
        }
    }
    else if ((int)inputQueue.size() == 2)
    {
        VelX = SPEED_X;
        direction.right = true;
        direction.left = false;
    }
    else
    {
        VelX = 0;
    }
}

void Player::action(Map map)
{
    if ((map.tile[(y + VelY) / TILE_SIZE + 1][(x + VelX) / TILE_SIZE] == 1 ||
        map.tile[(y + VelY) / TILE_SIZE + 1][(x + VelX + frameClip[curFrame].w) / TILE_SIZE]) == 1 &&
        VelY > 0)
    {
        VelY = 0;
    }

    if (VelY == 0 &&
        (map.tile[(y + VelY) / TILE_SIZE + 1][(x + VelX) / TILE_SIZE] != 1 && map.tile[(y + VelY) / TILE_SIZE + 1][(x + VelX + frameClip[curFrame].w) / TILE_SIZE] != 1))
    {
        VelY = GRAVITY;
    }

    if (VelY == 0 && onGround == false)
    {
        onGround = true;
        direction.up = false;
    }

    x += VelX;
    y += VelY;

    if (direction.up == true)
    {
        VelY += 1;
    }

    if (x < 0 || x > MAX_MAP_X * TILE_SIZE)
        x = 0;
    if (y < 0 || y > MAX_MAP_Y * TILE_SIZE)
        y = 0;
}
