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

void Enemy1::loadIMG(std::string path)
{
    free();

    Object::loadIMG(path);

    frameW = rect.w / numFrame;
    frameH = rect.h;

    for (int i = 0; i < numFrame; i++)
    {
        frameClip[i].x = i * frameW;
        frameClip[i].y = 0;
        frameClip[i].w = frameW;
        frameClip[i].h = frameH;
    }
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
    status = Enemy2Action::AIM_270_DEGREE;
    timer = 0;
}

Enemy2::~Enemy2()
{
}

void Enemy2::loadIMG(std::string path)
{
    free();

    Object::loadIMG(path);

    frameW = rect.w / numFrame;
    frameH = rect.h;

    for (int i = 0; i < numFrame; i++)
    {
        frameClip[i].x = i * frameW;
        frameClip[i].y = 0;
        frameClip[i].w = frameW;
        frameClip[i].h = frameH;
    }
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
            status = Enemy2Action::AIM_315_DEGREE;
        }
        else if (playerY >= y && playerY < (y + frameH))
        {
            status = Enemy2Action::AIM_270_DEGREE;
        }
        else
        {
            status = Enemy2Action::AIM_225_DEGREE;
        }
    }
    else
    {
        if (playerY < y)
        {
            status = Enemy2Action::AIM_45_DEGREE;
        }
        else if (playerY >= y && playerY < (y + frameH))
        {
            status = Enemy2Action::AIM_90_DEGREE;
        }
        else
        {
            status = Enemy2Action::AIM_135_DEGREE;
        }
    }
}

void Enemy2::createBullet(int playerX, int playerY)
{
    Bullet *newBullet = new Bullet();
    newBullet->loadIMG("res/img/bullet.png");

    if (status == Enemy2Action::AIM_270_DEGREE)
    {
        newBullet->setPos(x - 6, y + 31);
    }
    else if (status == Enemy2Action::AIM_90_DEGREE)
    {
        newBullet->setPos(x + frameW, y + 31);
    }
    else if (status == Enemy2Action::AIM_315_DEGREE)
    {
        newBullet->setPos(x, y);
    }
    else if (status == Enemy2Action::AIM_45_DEGREE)
    {
        newBullet->setPos(x + 48, y);
    }
    else if (status == Enemy2Action::AIM_225_DEGREE)
    {
        newBullet->setPos(x, y + 78);
    }
    else if (status == Enemy2Action::AIM_135_DEGREE)
    {
        newBullet->setPos(x + 71, y + 78);
    }

    newBullet->setVelX((playerX - newBullet->getX()) / sqrt((playerX - newBullet->getX()) * (playerX - newBullet->getX()) + (playerY - newBullet->getY()) * (playerY - newBullet->getY())) * ENEMY_BULLET_SPEED);
    newBullet->setVelY((playerY - newBullet->getY()) / sqrt((playerX - newBullet->getX()) * (playerX - newBullet->getX()) + (playerY - newBullet->getY()) * (playerY - newBullet->getY())) * ENEMY_BULLET_SPEED);

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

Enemy3::Enemy3()
{
    VelX = -ENEMY_SPEED_X;
    VelY = 0;
    x = 1000;
    y = 2 * TILE_SIZE;
    numFrame = 12;
    curFrame = 0;
    frameH = 0;
    frameW = 0;
    status = Enemy3Action::AIM_270_DEGREE;
    timer = 0;
}

Enemy3::~Enemy3()
{
}

void Enemy3::loadIMG(std::string path)
{
    free();

    Object::loadIMG(path);

    frameW = rect.w / numFrame;
    frameH = rect.h;

    for (int i = 0; i < numFrame; i++)
    {
        frameClip[i].x = i * frameW;
        frameClip[i].y = 0;
        frameClip[i].w = frameW;
        frameClip[i].h = frameH;
    }
}

void Enemy3::show()
{
    if (camera.x > x - SCREEN_WIDTH)
    {
        loadIMG("res/img/enemy3.png");
        curFrame = (int)status;

        rect.x = x - camera.x;
        rect.y = y - camera.y;

        SDL_Rect renderQuad = {rect.x, rect.y, frameW, frameH};

        SDL_Rect *curClip = &frameClip[curFrame];
        SDL_RenderCopy(renderer, texture, curClip, &renderQuad);
    }
}

void Enemy3::chooseStatus(int playerX, int playerY)
{
    int x1 = playerX - (x + frameW / 2);
    int y1 = playerY - (y + frameH / 2);

    double angle = acos(-y1 / sqrt(y1 * y1 + x1 * x1));

    if (x1 < 0)
        angle = 2 * M_PI - angle;


    if ((angle >= (345.0 / 180 * M_PI)) || (angle < (15.0 / 180 * M_PI)))
    {
        status = Enemy3Action::AIM_0_DEGREE;
    }
    else if ((angle >= (15.0 / 180 * M_PI)) && (angle < (45.0 / 180 * M_PI)))
    {
        status = Enemy3Action::AIM_30_DEGREE;
    }
    else if ((angle >= (45.0 / 180 * M_PI)) && (angle < (75.0 / 180 * M_PI)))
    {
        status = Enemy3Action::AIM_60_DEGREE;
    }
    else if ((angle >= (75.0 / 180 * M_PI)) && (angle < (105.0 / 180 * M_PI)))
    {
        status = Enemy3Action::AIM_90_DEGREE;
    }
    else if ((angle >= (105.0 / 180 * M_PI)) && (angle < (135.0 / 180 * M_PI)))
    {
        status = Enemy3Action::AIM_120_DEGREE;
    }
    else if ((angle >= (135.0 / 180 * M_PI)) && (angle < (165.0 / 180 * M_PI)))
    {
        status = Enemy3Action::AIM_150_DEGREE;
    }
    else if ((angle >= (165.0 / 180 * M_PI)) && (angle < (195.0 / 180 * M_PI)))
    {
        status = Enemy3Action::AIM_180_DEGREE;
    }
    else if ((angle >= (195.0 / 180 * M_PI)) && (angle < (225.0 / 180 * M_PI)))
    {
        status = Enemy3Action::AIM_210_DEGREE;
    }
    else if ((angle >= (225.0 / 180 * M_PI)) && (angle < (255.0 / 180 * M_PI)))
    {
        status = Enemy3Action::AIM_240_DEGREE;
    }
    else if ((angle >= (255.0 / 180 * M_PI)) && (angle < (285.0 / 180 * M_PI)))
    {
        status = Enemy3Action::AIM_270_DEGREE;
    }
    else if ((angle >= (285.0 / 180 * M_PI)) && (angle < (315.0 / 180 * M_PI)))
    {
        status = Enemy3Action::AIM_300_DEGREE;
    }
    else if ((angle >= (315.0 / 180 * M_PI)) && (angle < (345.0 / 180 * M_PI)))
    {
        status = Enemy3Action::AIM_330_DEGREE;
    }

}

void Enemy3::createBullet(int playerX, int playerY)
{
    Bullet *newBullet = new Bullet();
    newBullet->loadIMG("res/img/bullet.png");

    if (status == Enemy3Action::AIM_0_DEGREE)
    {
        newBullet->setPos(x + 42, y + 7);
        newBullet->setVelX(0);
        newBullet->setVelY(-ENEMY_BULLET_SPEED);
    }
    else if (status == Enemy3Action::AIM_90_DEGREE)
    {
        newBullet->setPos(x + 93, y + 42);
        newBullet->setVelX(ENEMY_BULLET_SPEED);
        newBullet->setVelY(0);
    }
    else if (status == Enemy3Action::AIM_30_DEGREE)
    {
        newBullet->setPos(x + 67, y);
        newBullet->setVelX(ENEMY_BULLET_SPEED * sin(M_PI / 6));
        newBullet->setVelY(-ENEMY_BULLET_SPEED * cos(M_PI / 6));
    }
    else if (status == Enemy3Action::AIM_60_DEGREE)
    {
        newBullet->setPos(x + 84, y + 18);
        newBullet->setVelX(ENEMY_BULLET_SPEED * sin(M_PI / 3));
        newBullet->setVelY(-ENEMY_BULLET_SPEED * cos(M_PI / 3));
    }
    else if (status == Enemy3Action::AIM_120_DEGREE)
    {
        newBullet->setPos(x + 83, y + 68);
        newBullet->setVelX(ENEMY_BULLET_SPEED * sin(M_PI * 2 / 3));
        newBullet->setVelY(-ENEMY_BULLET_SPEED * cos(M_PI * 2 / 3));
    }
    else if (status == Enemy3Action::AIM_150_DEGREE)
    {
        newBullet->setPos(x + 68, y + 84);
        newBullet->setVelX(ENEMY_BULLET_SPEED * sin(M_PI * 5 / 6));
        newBullet->setVelY(-ENEMY_BULLET_SPEED * cos(M_PI * 5 / 6));
    }
    else if (status == Enemy3Action::AIM_180_DEGREE)
    {
        newBullet->setPos(x + 42, y + 88);
        newBullet->setVelX(0);
        newBullet->setVelY(ENEMY_BULLET_SPEED);
    }
    else if (status == Enemy3Action::AIM_210_DEGREE)
    {
        newBullet->setPos(x + 18, y + 84);
        newBullet->setVelX(ENEMY_BULLET_SPEED * sin(M_PI * 7 / 6));
        newBullet->setVelY(-ENEMY_BULLET_SPEED * cos(M_PI * 7 / 6));
    }
    else if (status == Enemy3Action::AIM_240_DEGREE)
    {
        newBullet->setPos(x + 2, y + 69);
        newBullet->setVelX(ENEMY_BULLET_SPEED * sin(M_PI * 4 / 3));
        newBullet->setVelY(-ENEMY_BULLET_SPEED * cos(M_PI * 4 / 3));
    }
    else if (status == Enemy3Action::AIM_270_DEGREE)
    {
        newBullet->setPos(x, y + 42);
        newBullet->setVelX(ENEMY_BULLET_SPEED * sin(M_PI * 3 / 2));
        newBullet->setVelY(-ENEMY_BULLET_SPEED * cos(M_PI * 3 / 2));
    }
    else if (status == Enemy3Action::AIM_300_DEGREE)
    {
        newBullet->setPos(x + 2, y + 19);
        newBullet->setVelX(ENEMY_BULLET_SPEED * sin(M_PI * 5 / 3));
        newBullet->setVelY(-ENEMY_BULLET_SPEED * cos(M_PI * 5 / 3));
    }
    else if (status == Enemy3Action::AIM_330_DEGREE)
    {
        newBullet->setPos(x + 18, y + 2);
        newBullet->setVelX(ENEMY_BULLET_SPEED * sin(M_PI * 11 / 6));
        newBullet->setVelY(-ENEMY_BULLET_SPEED * cos(M_PI * 11 / 6));
    }

    newBullet->setOnScreen();

    bulletList.push_back(newBullet);
}

void Enemy3::handleBullet()
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

void Enemy3::action(Map map, int playerX, int playerY)
{
    if (camera.x > x - SCREEN_WIDTH)
    {
        if (timer > 30)
            timer = 0;
        chooseStatus(playerX, playerY);
        if (timer == 0)
            createBullet(playerX, playerY);
        handleBullet();
        timer++;
    }
}