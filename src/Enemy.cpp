#include "Enemy.h"

Enemy1::Enemy1()
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

Enemy1::~Enemy1()
{
}

bool Enemy1::loadIMG(std::string path)
{
    free();

    bool flag = Object::loadIMG(path);
    if (!flag)
        std::cout << "Error";

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

void Enemy1::show()
{
    if (camera.x > x - SCREEN_WIDTH)
    {
        loadIMG("res/img/enemy1.png");

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

void Enemy1::action(Map map)
{
    if (camera.x > x - SCREEN_WIDTH)
    {
        int x1 = x + VelX;
        int x2 = x1 + frameW;
        int y1 = y + VelY;
        int y2 = y1 + frameH;
        if ((map.tile[y2 / TILE_SIZE][x1 / TILE_SIZE] != 0 ||
             map.tile[y2 / TILE_SIZE][x2 / TILE_SIZE] != 0) &&
            VelY >= 0)
        {
            y = y2 / TILE_SIZE * TILE_SIZE - frameH + 15;
            VelY = 0;
            onGround = true;
        }

        if (VelY == 0 && onGround == true &&
            (map.tile[y2 / TILE_SIZE][x1 / TILE_SIZE] == 0 && map.tile[y2 / TILE_SIZE][x2 / TILE_SIZE] == 0))
        {
            VelY = GRAVITY;
            onGround = false;
        }

        x += VelX;
        y += VelY;
    }
}

Enemy2::Enemy2()
{
    VelX = -ENEMY_SPEED_X;
    VelY = 0;
    x = 1000;
    y = 2 * TILE_SIZE;
    numFrame = 6;
    curFrame = 0;
    frameH = 0;
    frameW = 0;
    status = Enemy1Action::AIM_LEFT;
    timer = 0;
}

Enemy2::~Enemy2()
{
}

bool Enemy2::loadIMG(std::string path)
{
    free();

    bool flag = Object::loadIMG(path);
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

void Enemy2::show()
{
    if (camera.x > x - SCREEN_WIDTH)
    {
        loadIMG("res/img/enemy2.png");
        curFrame = (int)status;

        rect.x = x - camera.x;
        rect.y = y - camera.y;

        SDL_Rect renderQuad = {rect.x, rect.y, frameW, frameH};

        SDL_Rect *curClip = &frameClip[curFrame];
        SDL_RenderCopy(renderer, texture, curClip, &renderQuad);
    }
}

void Enemy2::chooseStatus(int playerX, int playerY)
{
    if (playerX < (x + frameW / 2))
    {
        if (playerY < y)
        {
            status = Enemy1Action::AIM_UP_LEFT;
        }
        else if (playerY >= y && playerY < (y + frameH))
        {
            status = Enemy1Action::AIM_LEFT;
        }
        else
        {
            status = Enemy1Action::AIM_DOWN_LEFT;
        }
    }
    else
    {
        if (playerY < y)
        {
            status = Enemy1Action::AIM_UP_RIGHT;
        }
        else if (playerY >= y && playerY < (y + frameH))
        {
            status = Enemy1Action::AIM_RIGHT;
        }
        else
        {
            status = Enemy1Action::AIM_DOWN_RIGHT;
        }
    }
}

void Enemy2::createBullet(int playerX, int playerY)
{
    Bullet *newBullet = new Bullet();
    newBullet->loadIMG("res/img/bullet.png");

    if (status == Enemy1Action::AIM_LEFT)
    {
        newBullet->setPos(x - 6, y + 31);
    }
    else if (status == Enemy1Action::AIM_RIGHT)
    {
        newBullet->setPos(x + frameW, y + 31);
    }
    else if (status == Enemy1Action::AIM_UP_LEFT)
    {
        newBullet->setPos(x, y);
    }
    else if (status == Enemy1Action::AIM_UP_RIGHT)
    {
        newBullet->setPos(x + 48, y);
    }
    else if (status == Enemy1Action::AIM_DOWN_LEFT)
    {
        newBullet->setPos(x, y + 78);
    }
    else if (status == Enemy1Action::AIM_DOWN_RIGHT)
    {
        newBullet->setPos(x + 71, y + 78);
    }

    newBullet->setVelX((playerX - newBullet->getX()) / sqrt((playerX - newBullet->getX()) * (playerX - newBullet->getX()) + (playerY - newBullet->getY()) * (playerY - newBullet->getY())) * BULLET_SPEED);
    newBullet->setVelY((playerY - newBullet->getY()) / sqrt((playerX - newBullet->getX()) * (playerX - newBullet->getX()) + (playerY - newBullet->getY()) * (playerY - newBullet->getY())) * BULLET_SPEED);


    newBullet->setOnScreen();

    bulletList.push_back(newBullet);
}

void Enemy2::handleBullet()
{
    for (int i = 0; i < bulletList.size(); i++)
    {
        if (bulletList[i] != NULL)
        {
            if (bulletList[i]->getOnScreen() == true)
            {
                bulletList[i]->move();
                bulletList[i]->renderBullet();
            }
            else
            {
                bulletList[i]->free();
                delete bulletList[i];
                bulletList.erase(bulletList.begin() + i);
            }
        }
    }
}

void Enemy2::action(Map map, int playerX, int playerY)
{
    if (camera.x > x - SCREEN_WIDTH)
    {
        if (timer > 30)
            timer = 0;
        chooseStatus(playerX, playerY);
        if (timer == 0 || timer == 3 || timer == 6)
            createBullet(playerX, playerY);
        handleBullet();
        timer++;
    }
}
