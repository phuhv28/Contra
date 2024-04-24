#include "Player.h"

Player::Player()
{
    x = 100;
    y = 0;
    curFrame = 0;
    frameH = 0;
    frameW = 0;
    VelX = 0;
    VelY = GRAVITY;
    direction.right = true;
    status.onGround = false;
    aCurFrame = 0;
    this->camera = camera;
    status.isFalling = false;
    numFrame = 1;
    status.isFiring = false;
    status.action = Action::JUMPING;
    fireSound = NULL;
    timer = 0;
    lives = 3;
}

Player::~Player()
{
}

void Player::loadIMG(std::string path)
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

void Player::removeBullet(int index)
{
    int size = bullet.size();
    if (size > 0 && index < size)
    {
        if (bullet[index] != NULL)
        {
            bullet[index]->free();
            delete bullet[index];
            bullet.erase(bullet.begin() + index);
        }
    }
}

void Player::show()
{
    switch (status.action)
    {
    case Action::DEAD:
        numFrame = 5;
        loadIMG("res/img/die.png");
        break;
    case Action::JUMPING:
        numFrame = 3;
        loadIMG("res/img/jump.png");
        break;
    case Action::AIM_UP_RIGHT_WHILE_WALKING:
        numFrame = 3;
        loadIMG("res/img/up_right_and_walking.png");
        break;
    case Action::AIM_UP_LEFT_WHILE_WALKING:
        numFrame = 3;
        loadIMG("res/img/up_left_and_walking.png");
        break;
    case Action::AIM_DOWN_LEFT_WHILE_WALKING:
        numFrame = 3;
        loadIMG("res/img/down_left.png");
        break;
    case Action::AIM_DOWN_RIGHT_WHILE_WALKING:
        numFrame = 3;
        loadIMG("res/img/down_right.png");
        break;
    case Action::STANDING_LEFT:
        numFrame = 1;
        loadIMG("res/img/standingL.png");
        break;
    case Action::STANDING_RIGHT:
        numFrame = 1;
        loadIMG("res/img/standingR.png");
        break;
    case Action::WALKING_LEFT:
        numFrame = 3;
        loadIMG("res/img/walkingL.png");
        break;
    case Action::WALKING_RIGHT:
        numFrame = 3;
        loadIMG("res/img/walkingR.png");
        break;
    case Action::LAYING_DOWN_LEFT:
        numFrame = 1;
        loadIMG("res/img/laydownL.png");
        break;
    case Action::LAYING_DOWN_RIGHT:
        numFrame = 1;
        loadIMG("res/img/laydownR.png");
        break;
    case Action::AIM_UP_RIGHT:
        numFrame = 1;
        loadIMG("res/img/upR.png");
        break;
    case Action::AIM_UP_LEFT:
        numFrame = 1;
        loadIMG("res/img/upL.png");
        break;
    case Action::AIM_RIGHT_WHILE_WALKING:
        numFrame = 3;
        loadIMG("res/img/firing_while_walkingR.png");
        break;
    case Action::AIM_LEFT_WHILE_WALKING:
        numFrame = 3;
        loadIMG("res/img/firing_while_walkingL.png");
        break;
    }

    if ((int)inputQueue.size() != 0 || status.onGround == false || isDead())
    {
        aCurFrame++;
        if (aCurFrame >= numFrame * SLOWMOTION_ANIMATION_RATE)
            aCurFrame = 0;
    }
    else
    {
        aCurFrame = (numFrame - 1) * SLOWMOTION_ANIMATION_RATE;
    }

    curFrame = aCurFrame / SLOWMOTION_ANIMATION_RATE;

    SDL_Rect *curClip = &frameClip[curFrame];

    if (x < Object::camera.x)
        x = Object::camera.x;
    if (x > Object::camera.x + SCREEN_WIDTH)
        x = Object::camera.x + SCREEN_WIDTH;
    if (y < Object::camera.y)
        y = Object::camera.y;
    if (y > Object::camera.y + SCREEN_HEIGHT)
        y = Object::camera.y + SCREEN_HEIGHT;

    rect.x = x - Object::camera.x;
    rect.y = y - Object::camera.y;

    SDL_Rect renderQuad = {rect.x, rect.y, frameW, frameH};

    SDL_RenderCopy(renderer, texture, curClip, &renderQuad);
}

void Player::getInput(SDL_Event e, Mix_Chunk *fireSound)
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
        case SDLK_UP:
            inputQueue.push_back(Input::UP);
            break;
        case SDLK_DOWN:
            inputQueue.push_back(Input::DOWN);
            break;
        case SDLK_x:
            if (status.onGround == true)
            {
                bool flag = false;
                for (int i = (int)inputQueue.size() - 1; i >= 0; i--)
                {
                    if (inputQueue[i] == Input::DOWN)
                    {
                        status.isFalling = true;
                        status.onGround = false;
                        if (direction.right == true)
                            status.action = Action::STANDING_RIGHT;
                        if (direction.left == true)
                            status.action = Action::STANDING_LEFT;
                        VelY = GRAVITY;
                        flag = true;
                        break;
                    }
                }
                if (!flag)
                {
                    status.action = Action::JUMPING;
                    status.onGround = false;
                    VelY -= 17;
                }
            }
            break;
        case SDLK_z:
            if (timer == RELOAD)
            {
                timer = 0;
                status.isFiring = true;
                Mix_PlayChannel(-1, fireSound, 0);
                clock.start();
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
            for (int i = 0; i < (int)inputQueue.size(); i++)
            {
                if (inputQueue[i] == Input::DOWN)
                    inputQueue.erase(inputQueue.begin() + i);
            }

            break;
        case SDLK_UP:
            for (int i = 0; i < (int)inputQueue.size(); i++)
            {
                if (inputQueue[i] == Input::UP)
                    inputQueue.erase(inputQueue.begin() + i);
            }
            break;
        case SDLK_z:
            status.isFiring = false;
            break;
        }
    }
}

void Player::handleInputQueue(SDL_Event e)
{
    if ((int)inputQueue.size() != 0)
    {

        if (inputQueue.size() > 2)
        {
            std::vector<Input> inputQueue_;
            for (int i = 0; i < (int)inputQueue.size(); i++)
            {
                if (inputQueue[i] == Input::LEFT || inputQueue[i] == Input::RIGHT)
                    inputQueue_.push_back(inputQueue[i]);
            }

            if ((int)inputQueue_.size() == 1)
            {
                if (inputQueue_[0] == Input::RIGHT)
                {
                    VelX = PLAYER_SPEED_X;
                    direction.right = true;
                    direction.left = false;
                    if (status.onGround == true)
                        status.action = Action::WALKING_RIGHT;
                }
                else
                {
                    VelX = -PLAYER_SPEED_X;
                    direction.left = true;
                    direction.right = false;
                    if (status.onGround == true)
                        status.action = Action::WALKING_LEFT;
                }
            }
            else if ((int)inputQueue_.size() == 2)
            {
                VelX = PLAYER_SPEED_X;
                direction.right = true;
                direction.left = false;
                if (status.onGround == true)
                    status.action = Action::WALKING_RIGHT;
            }
        }
        else if ((int)inputQueue.size() == 2)
        {
            if ((inputQueue[0] == Input::DOWN && inputQueue[1] == Input::LEFT) || (inputQueue[0] == Input::LEFT && inputQueue[1] == Input::DOWN))
            {
                VelX = -PLAYER_SPEED_X;
                direction.right = false;
                direction.left = true;
                if (status.onGround == true)
                    status.action = Action::AIM_DOWN_LEFT_WHILE_WALKING;
            }
            if ((inputQueue[0] == Input::DOWN && inputQueue[1] == Input::RIGHT) || (inputQueue[0] == Input::RIGHT && inputQueue[1] == Input::DOWN))
            {
                VelX = PLAYER_SPEED_X;
                direction.right = true;
                direction.left = false;
                if (status.onGround == true)
                    status.action = Action::AIM_DOWN_RIGHT_WHILE_WALKING;
            }
            if ((inputQueue[0] == Input::UP && inputQueue[1] == Input::LEFT) || (inputQueue[0] == Input::LEFT && inputQueue[1] == Input::UP))
            {
                VelX = -PLAYER_SPEED_X;
                direction.right = false;
                direction.left = true;
                if (status.onGround == true)
                    status.action = Action::AIM_UP_LEFT_WHILE_WALKING;
            }
            if ((inputQueue[0] == Input::UP && inputQueue[1] == Input::RIGHT) || (inputQueue[0] == Input::RIGHT && inputQueue[1] == Input::UP))
            {
                VelX = PLAYER_SPEED_X;
                direction.right = true;
                direction.left = false;
                if (status.onGround == true)
                    status.action = Action::AIM_UP_RIGHT_WHILE_WALKING;
            }
            if ((inputQueue[0] == Input::LEFT && inputQueue[1] == Input::RIGHT) || (inputQueue[0] == Input::RIGHT && inputQueue[1] == Input::LEFT))
            {
                VelX = PLAYER_SPEED_X;
                direction.right = true;
                direction.left = false;
                if (status.onGround == true)
                    status.action = Action::WALKING_RIGHT;
            }
        }
        else
        {
            if (inputQueue[0] == Input::DOWN)
            {
                if (status.onGround == true)
                {
                    if (direction.left == true)
                        status.action = Action::LAYING_DOWN_LEFT;
                    else if (direction.right == true)
                        status.action = Action::LAYING_DOWN_RIGHT;
                    VelX = 0;
                }
            }
            else if (inputQueue[0] == Input::UP)
            {
                direction.up = true;
                if (status.onGround == true)
                {
                    if (direction.left == true)
                        status.action = Action::AIM_UP_LEFT;
                    else if (direction.right == true)
                        status.action = Action::AIM_UP_RIGHT;
                    VelX = 0;
                }
            }
            else if (inputQueue[0] == Input::RIGHT)
            {
                VelX = PLAYER_SPEED_X;
                direction.right = true;
                direction.left = false;
                if (status.onGround == true)
                    status.action = Action::WALKING_RIGHT;
            }
            else if (inputQueue[0] == Input::LEFT)
            {
                VelX = -PLAYER_SPEED_X;
                direction.right = false;
                direction.left = true;
                if (status.onGround == true)
                    status.action = Action::WALKING_LEFT;
            }
        }
    }
    else
    {
        if (status.action != Action::DEAD)
        {
            VelX = 0;
            if (status.onGround == true)
            {
                if (direction.right == true)
                    status.action = Action::STANDING_RIGHT;
                if (direction.left == true)
                    status.action = Action::STANDING_LEFT;
            }
        }
    }

    if (clock.getisStarted() == true)
    {
        if (clock.getTick().count() > 400)
        {
            clock.stop();
        }
    }

    if (clock.getisStarted() == true && (status.action == Action::WALKING_LEFT || status.action == Action::WALKING_RIGHT))
    {
        if (status.action == Action::WALKING_LEFT)
            status.action = Action::AIM_LEFT_WHILE_WALKING;
        if (status.action == Action::WALKING_RIGHT)
            status.action = Action::AIM_RIGHT_WHILE_WALKING;
    }
    else if (clock.getisStarted() == true)
    {
        clock.stop();
    }

    if (status.isFiring == true)
    {
        status.isFiring = false;
        createBullet();
    }
}

void Player::action(Map map)
{
    if (isDead())
        VelX = 0;

    timer++;
    if (timer >= RELOAD)
        timer = RELOAD;
    x = x + frameClip[curFrame].w - w[(int)status.action];
    y = y + frameClip[curFrame].h - h[(int)status.action];

    int x1 = x + VelX;
    int x2 = x1 + w[(int)status.action];
    int y1 = y + VelY;
    int y2 = y1 + h[(int)status.action];
    if ((map.tile[y2 / TILE_SIZE][x1 / TILE_SIZE] != 0 ||
         map.tile[y2 / TILE_SIZE][x2 / TILE_SIZE] != 0) &&
        VelY >= 0 && (y + frameClip[curFrame].h) < (y2 / TILE_SIZE * TILE_SIZE) && status.isFalling == false)
    {
        y = y2 / TILE_SIZE * TILE_SIZE - h[(int)status.action] + 15;
        VelY = 0;
        status.onGround = true;
    }

    if (status.isFalling == true)
        status.isFalling = false;

    // Fall when go out of block 1
    if (VelY == 0 && status.onGround == true &&
        (map.tile[y2 / TILE_SIZE][x1 / TILE_SIZE] == 0 && map.tile[y2 / TILE_SIZE][x2 / TILE_SIZE] == 0))
    {
        VelY = GRAVITY;
        status.onGround = false;
    }

    x += VelX;
    y += VelY;

    if (status.onGround == false)
        VelY += 1;

    if (x < 0 || x > MAX_MAP_X * TILE_SIZE)
        x = 0;
    if (y > MAX_MAP_Y * TILE_SIZE)
    {
        x = Object::camera.x + TILE_SIZE / 2;
        y = TILE_SIZE;
        VelY = GRAVITY;
    }
}

void Player::createBullet()
{
    Bullet *newBullet = new Bullet();
    newBullet->loadIMG("res/img/bullet.png");

    // set start of bullet

    if (status.action == Action::STANDING_LEFT)
    {
        newBullet->setPos(x + PLAYER_BULLET_SPEED, y + 30);
        newBullet->setVelX(-PLAYER_BULLET_SPEED);
        newBullet->setVelY(0);
    }
    else if (status.action == Action::STANDING_RIGHT)
    {
        newBullet->setPos(x + 72 - PLAYER_BULLET_SPEED, y + 30);
        newBullet->setVelX(PLAYER_BULLET_SPEED);
        newBullet->setVelY(0);
    }
    else if (status.action == Action::AIM_DOWN_LEFT_WHILE_WALKING)
    {
        newBullet->setPos(x + PLAYER_BULLET_SPEED * cos(M_PI * 1 / 5), y - PLAYER_BULLET_SPEED * sin(M_PI * 1 / 5) + 67);
        newBullet->setVelX(-PLAYER_BULLET_SPEED * cos(M_PI * 1 / 5));
        newBullet->setVelY(PLAYER_BULLET_SPEED * sin(M_PI * 1 / 5));
    }
    else if (status.action == Action::AIM_DOWN_RIGHT_WHILE_WALKING)
    {
        newBullet->setPos(x + 69 - PLAYER_BULLET_SPEED * cos(M_PI * 1 / 5), y - PLAYER_BULLET_SPEED * sin(M_PI * 1 / 5) + 67);
        newBullet->setVelX(PLAYER_BULLET_SPEED * cos(M_PI * 1 / 5));
        newBullet->setVelY(PLAYER_BULLET_SPEED * sin(M_PI * 1 / 5));
    }
    else if (status.action == Action::AIM_UP_LEFT_WHILE_WALKING)
    {
        newBullet->setPos(x + PLAYER_BULLET_SPEED * cos(M_PI * 1 / 5), y + PLAYER_BULLET_SPEED * sin(M_PI * 1 / 5) + 3);
        newBullet->setVelX(-PLAYER_BULLET_SPEED * cos(M_PI * 1 / 5));
        newBullet->setVelY(-PLAYER_BULLET_SPEED * sin(M_PI * 1 / 5));
    }
    else if (status.action == Action::AIM_UP_RIGHT_WHILE_WALKING)
    {
        newBullet->setPos(x + 63 - PLAYER_BULLET_SPEED * cos(M_PI * 1 / 5), y + PLAYER_BULLET_SPEED * sin(M_PI * 1 / 5) + 3);
        newBullet->setVelX(PLAYER_BULLET_SPEED * cos(M_PI * 1 / 5));
        newBullet->setVelY(-PLAYER_BULLET_SPEED * sin(M_PI * 1 / 5));
    }
    else if (status.action == Action::AIM_UP_LEFT)
    {
        newBullet->setPos(x + 6, y + PLAYER_BULLET_SPEED);
        newBullet->setVelX(0);
        newBullet->setVelY(-PLAYER_BULLET_SPEED);
    }
    else if (status.action == Action::AIM_UP_RIGHT)
    {
        newBullet->setPos(x + 26, y + PLAYER_BULLET_SPEED);
        newBullet->setVelX(0);
        newBullet->setVelY(-PLAYER_BULLET_SPEED);
    }
    else if (status.action == Action::LAYING_DOWN_LEFT)
    {
        newBullet->setPos(x + PLAYER_BULLET_SPEED, y + 19);
        newBullet->setVelX(-PLAYER_BULLET_SPEED);
        newBullet->setVelY(0);
    }
    else if (status.action == Action::LAYING_DOWN_RIGHT)
    {
        newBullet->setPos(x + 102 - PLAYER_BULLET_SPEED, y + 19);
        newBullet->setVelX(PLAYER_BULLET_SPEED);
        newBullet->setVelY(0);
    }
    else if (status.action == Action::JUMPING)
    {
        newBullet->setPos(x + 30 - PLAYER_BULLET_SPEED, y + 30);
        if (direction.left == true)
            newBullet->setVelX(-PLAYER_BULLET_SPEED);
        if (direction.right == true)
            newBullet->setVelX(PLAYER_BULLET_SPEED);
    }
    else if (status.action == Action::AIM_RIGHT_WHILE_WALKING)
    {
        newBullet->setPos(x + 81 - PLAYER_BULLET_SPEED, y + 30);
        newBullet->setVelX(PLAYER_BULLET_SPEED);
        newBullet->setVelY(0);
    }
    else if (status.action == Action::AIM_LEFT_WHILE_WALKING)
    {
        newBullet->setPos(x + PLAYER_BULLET_SPEED, y + 30);
        newBullet->setVelX(-PLAYER_BULLET_SPEED);
        newBullet->setVelY(0);
    }
    else
        newBullet->setPos(x, y);

    newBullet->setOnScreen();

    bullet.push_back(newBullet);
}

void Player::handleBullet()
{
    for (int i = 0; i < bullet.size(); i++)
    {
        if (bullet[i] != NULL)
        {
            if (bullet[i]->getOnScreen() == true)
            {
                bullet[i]->move();
                bullet[i]->renderBullet();
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
