#include "Player.h"

Player::Player(const SDL_Rect *camera)
{
    x = 100;
    y = 0;
    curFrame = 0;
    VelX = 0;
    VelY = GRAVITY;
    direction.right = true;
    status.onGround = false;
    aCurFrame = 0;
    this->camera = camera;
    status.isFalling = false;
    numFrame = 1;
    status.isFiring = false;
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

void Player::show(SDL_Renderer *renderer)
{

    if (status.onGround == false)
    {
        numFrame = 3;
        loadIMG("res/jump.png", renderer);
    }
    else
    {
        if (direction.left == true)
        {
            if (direction.down == true)
            {
                numFrame = 1;
                loadIMG("res/laydownL.png", renderer);
            }
            else if (VelX < 0)
            {
                if (status.isFiring == true)
                {
                    numFrame = 3;
                    loadIMG("res/firing_while_walkingL.png", renderer);
                }
                else
                {
                    numFrame = 3;
                    loadIMG("res/walkingL.png", renderer);
                }
            }
            else
            {
                numFrame = 1;
                loadIMG("res/standingL.png", renderer);
            }
        }
        else if (direction.right == true)
        {
            if (direction.down == true)
            {
                numFrame = 1;
                loadIMG("res/laydownR.png", renderer);
            }
            else if (VelX > 0)
            {
                if (status.isFiring == true)
                {
                    numFrame = 3;
                    loadIMG("res/firing_while_walkingR.png", renderer);
                }
                else
                {
                    numFrame = 3;
                    loadIMG("res/walkingR.png", renderer);
                }
            }
            else
            {
                numFrame = 1;
                loadIMG("res/standingR.png", renderer);
            }
        }
    }
    if ((int)inputQueue.size() != 0 || status.onGround == false)
    {
        aCurFrame++;
        if (aCurFrame >= numFrame * SLOWMOTION_ANIMATION_RATE)
            aCurFrame = 0;
    }
    else
    {
        aCurFrame = (numFrame - 1) * SLOWMOTION_ANIMATION_RATE;
    }
    std::cout << "aCurFrame: " << aCurFrame << " \n";

    curFrame = aCurFrame / SLOWMOTION_ANIMATION_RATE;
    std::cout << "curFrame: " << curFrame << " \n";

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
            if (status.onGround == true)
            {
                if (direction.down == false)
                {
                    direction.up = true;
                    VelY = -17;
                    status.onGround = false;
                    direction.down = false;
                }
                else
                {
                    status.isFalling = true;
                    VelY = GRAVITY;
                }
            }
            break;
        case SDLK_z:
            createBullet(renderer);
            status.isFiring = true;
            break;
        case SDLK_DOWN:
            if (status.onGround == true)
            {
                direction.down = true;
                y += 57;
            }
            break;
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
        case SDLK_DOWN:
            if (direction.down == true)
            {
                direction.down = false;
                y -= 57;
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
    // Can't move when lay down
    if (direction.down == true)
        VelX = 0;

    if (status.isFalling == true)
        y -= 57;

    int x1 = x + VelX;
    int x2 = x1 + frameClip[curFrame].w;
    int y1 = y + VelY;
    int y2 = y1 + PLAYER_HEIGHT + 15;

    // Check horizontal collision only when VelY > 0
    if ((map.tile[y2 / TILE_SIZE][x1 / TILE_SIZE] == 1 ||
         map.tile[y2 / TILE_SIZE][x2 / TILE_SIZE] == 1) &&
        VelY > 0 && (y + PLAYER_HEIGHT + 15) < (y2 / TILE_SIZE * TILE_SIZE) && status.isFalling == false)
    {
        y = y2 / TILE_SIZE * TILE_SIZE - PLAYER_HEIGHT + 15;
        VelY = 0;
        status.onGround = true;
        direction.up = false;
    }

    if (status.isFalling == true)
    {
        status.isFalling = false;
        direction.down = false;
    }

    // Fall when go out of block 1
    if (VelY == 0 && status.onGround == true &&
        (map.tile[y2 / TILE_SIZE][x1 / TILE_SIZE] != 1 && map.tile[y2 / TILE_SIZE][x2 / TILE_SIZE] != 1))
    {
        VelY = GRAVITY;
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

void Player::createBullet(SDL_Renderer *renderer)
{
    Bullet *newBullet = new Bullet();
    newBullet->loadIMG("res/bullet.png", renderer);
    newBullet->setPos(x, y + 14);
    if (direction.left == true)
        newBullet->setVelX(-BULLET_SPEED);
    if (direction.right == true)
        newBullet->setVelX(BULLET_SPEED);
    newBullet->setOnScreen();

    bullet.push_back(newBullet);
}

void Player::handleBullet(SDL_Renderer *renderer)
{
    for (int i = 0; i < bullet.size(); i++)
    {
        if (bullet[i] != NULL)
        {
            if (bullet[i]->getOnScreen() == true)
            {
                bullet[i]->move(camera);
                bullet[i]->renderBullet(renderer, camera);
            }
            else
            {
                bullet[i]->free();
                delete bullet[i];
                bullet.erase(bullet.begin() + i);
            }
        }
    }
}
