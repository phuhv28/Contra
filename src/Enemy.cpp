#include "Enemy.h"

Enemy::Enemy()
{
    VelX = -ENEMY_SPEED_X;
    VelY = 0;
    x = 1000;
    y = 2 * TILE_SIZE;
    numFrame = 6;
    curFrame = 0;
    aCurFrame = 0;
    onGround = true;
    frameH = 0;
    frameW = 0;
    // this->player = player;
}

Enemy::~Enemy()
{
}

bool Enemy::loadIMG(std::string path, SDL_Renderer *renderer)
{
    free();

    bool flag = Object::loadIMG(path, renderer);
    if (!flag)
        std::cout << "Error!";

    frameW = rect.w / numFrame;
    frameH = rect.h;

    for (int i = 0; i < numFrame; i++)
    {
        frameClip[i].x = i * frameW;
        frameClip[i].y = 0;
        frameClip[i].w = frameW;
        frameClip[i].h = frameH;
    }

    return flag;
}

void Enemy::show(SDL_Renderer *renderer, const SDL_Rect &camera)
{
    if (camera.x > x - SCREEN_WIDTH)
    {
        loadIMG("res/enemy1.png", renderer);

        aCurFrame++;
        if (aCurFrame >= numFrame * SLOWMOTION_ANIMATION_RATE)
            aCurFrame = 0;

        curFrame = aCurFrame / SLOWMOTION_ANIMATION_RATE;

        SDL_Rect *curClip = &frameClip[curFrame];

        rect.x = x - camera.x;
        rect.y = y - camera.y;

        SDL_Rect renderQuad = {rect.x, rect.y, frameW, frameH};

        SDL_RenderCopy(renderer, texture, curClip, &renderQuad);
    }
}

void Enemy::action(Map map, const SDL_Rect &camera)
{
    if (camera.x > x - SCREEN_WIDTH)
    {
        int x1 = x + VelX;
        int x2 = x1 + frameW;
        int y1 = y + VelY;
        int y2 = y1 + frameH;
        if ((map.tile[y2 / TILE_SIZE][x1 / TILE_SIZE] == 1 ||
             map.tile[y2 / TILE_SIZE][x2 / TILE_SIZE] == 1) &&
            VelY >= 0)
        {
            y = y2 / TILE_SIZE * TILE_SIZE - frameH + 15;
            VelY = 0;
            onGround = true;
        }

        if (VelY == 0 && onGround == true &&
            (map.tile[y2 / TILE_SIZE][x1 / TILE_SIZE] != 1 && map.tile[y2 / TILE_SIZE][x2 / TILE_SIZE] != 1))
        {
            VelY = GRAVITY;
            onGround = false;
        }

        x += VelX;
        y += VelY;

    }
}
