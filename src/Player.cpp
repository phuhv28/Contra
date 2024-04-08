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
}

Player::~Player()
{
}

void Player::setCam(const SDL_Rect &camera)
{
    this->camera = &camera;
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
    // std::cout << x << " " << y << std::endl;
    switch (status.action)
    {
    case Action::DEAD:
        numFrame = 5;
        loadIMG("res/die.png", renderer);
        break;
    case Action::JUMPING:
        numFrame = 3;
        loadIMG("res/jump.png", renderer);
        break;
    case Action::AIM_UP_RIGHT_WHILE_WALKING:
        numFrame = 3;
        loadIMG("res/up_right_and_walking.png", renderer);
        break;
    case Action::AIM_UP_LEFT_WHILE_WALKING:
        numFrame = 3;
        loadIMG("res/up_left_and_walking.png", renderer);
        break;
    case Action::AIM_DOWN_LEFT_WHILE_WALKING:
        numFrame = 3;
        loadIMG("res/down_left.png", renderer);
        break;
    case Action::AIM_DOWN_RIGHT_WHILE_WALKING:
        numFrame = 3;
        loadIMG("res/down_right.png", renderer);
        break;
    case Action::STANDING_LEFT:
        numFrame = 1;
        loadIMG("res/standingL.png", renderer);
        break;
    case Action::STANDING_RIGHT:
        numFrame = 1;
        loadIMG("res/standingR.png", renderer);
        break;
    case Action::WALKING_LEFT:
        numFrame = 3;
        loadIMG("res/walkingL.png", renderer);
        break;
    case Action::WALKING_RIGHT:
        numFrame = 3;
        loadIMG("res/walkingR.png", renderer);
        break;
    case Action::LAYING_DOWN_LEFT:
        numFrame = 1;
        loadIMG("res/laydownL.png", renderer);
        break;
    case Action::LAYING_DOWN_RIGHT:
        numFrame = 1;
        loadIMG("res/laydownR.png", renderer);
        break;
    case Action::AIM_UP_RIGHT:
        numFrame = 1;
        loadIMG("res/upR.png", renderer);
        break;
    case Action::AIM_UP_LEFT:
        numFrame = 1;
        loadIMG("res/upL.png", renderer);
        break;
    case Action::AIM_RIGHT_WHILE_WALKING:
        numFrame = 3;
        loadIMG("res/firing_while_walkingR.png", renderer);
        break;
    case Action::AIM_LEFT_WHILE_WALKING:
        numFrame = 3;
        loadIMG("res/firing_while_walkingL.png", renderer);
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
    std::cout << curFrame << " ";

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

void Player::getInput(SDL_Event e, SDL_Renderer *renderer)
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
            status.isFiring = true;
            clock.start();
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

void Player::handleInputQueue(SDL_Event e, SDL_Renderer *renderer)
{
    // std::cout << inputQueue.size();
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
            // std::cout << 1;
        }
    }

    if (clock.getisStarted() == true && (status.action == Action::WALKING_LEFT || status.action == Action::WALKING_RIGHT))
    {
        if (status.action == Action::WALKING_LEFT)
            status.action = Action::AIM_LEFT_WHILE_WALKING;
        if (status.action == Action::WALKING_RIGHT)
            status.action = Action::AIM_RIGHT_WHILE_WALKING;
        // std::cout << 2 << " " << (int) status.action << std::endl;
    }
    else if (clock.getisStarted() == true)
    {
        clock.stop();
        // std::cout << 3;
    }

    // if (status.action == Action::WALKING_LEFT && status.isFiring == true)
    //     status.action = Action::AIM_LEFT_WHILE_WALKING;
    // if (status.action == Action::WALKING_RIGHT && status.isFiring == true)
    //     status.action = Action::AIM_RIGHT_WHILE_WALKING;

    if (status.isFiring == true)
    {
        status.isFiring = false;
        createBullet(renderer);
    }
}

void Player::action(Map map)
{
    // std::cout << (int) status.action;

    x = x + frameClip[curFrame].w - w[(int)status.action];
    y = y + frameClip[curFrame].h - h[(int)status.action];

    int x1 = x + VelX;
    int x2 = x1 + w[(int)status.action];
    int y1 = y + VelY;
    int y2 = y1 + h[(int)status.action];
    // std::cout << std::boolalpha << status.isFalling << " " << status.onGround << std::endl;
    if ((map.tile[y2 / TILE_SIZE][x1 / TILE_SIZE] == 1 ||
         map.tile[y2 / TILE_SIZE][x2 / TILE_SIZE] == 1) &&
        VelY >= 0 && (y + frameClip[curFrame].h) < (y2 / TILE_SIZE * TILE_SIZE) && status.isFalling == false)
    {
        // std::cout << h[(int)status.action] << " ";
        y = y2 / TILE_SIZE * TILE_SIZE - h[(int)status.action] + 15;
        VelY = 0;
        status.onGround = true;
    }

    if (status.isFalling == true)
        status.isFalling = false;

    // Fall when go out of block 1
    if (VelY == 0 && status.onGround == true &&
        (map.tile[y2 / TILE_SIZE][x1 / TILE_SIZE] != 1 && map.tile[y2 / TILE_SIZE][x2 / TILE_SIZE] != 1))
    {
        // std::cout << "X";
        VelY = GRAVITY;
        status.onGround = false;
    }

    x += VelX;
    y += VelY;

    if (status.onGround == false)
        VelY += 1;

    if (x < 0 || x > MAX_MAP_X * TILE_SIZE)
        x = 0;
    if (y < 0 || y > MAX_MAP_Y * TILE_SIZE)
    {
        x = camera->x + TILE_SIZE / 2;
        y = TILE_SIZE;
        VelY = GRAVITY;
    }
}

void Player::createBullet(SDL_Renderer *renderer)
{
    // std::cout << "VelX: " << VelX << std::endl;
    Bullet *newBullet = new Bullet();
    newBullet->loadIMG("res/bullet.png", renderer);

    // set start of bullet
    // std::cout << x << " : " << y << std::endl;

    if (status.action == Action::STANDING_LEFT)
    {
        newBullet->setPos(x + BULLET_SPEED, y + 30);
        newBullet->setVelX(-BULLET_SPEED);
        newBullet->setVelY(0);
    }
    else if (status.action == Action::STANDING_RIGHT)
    {
        newBullet->setPos(x + 72 - BULLET_SPEED, y + 30);
        newBullet->setVelX(BULLET_SPEED);
        newBullet->setVelY(0);
    }
    else if (status.action == Action::AIM_DOWN_LEFT_WHILE_WALKING)
    {
        newBullet->setPos(x + BULLET_SPEED * cos(M_PI * 1 / 8), y - BULLET_SPEED * sin(M_PI * 7 / 8) + 67);
        newBullet->setVelX(-BULLET_SPEED * cos(M_PI * 1 / 8));
        newBullet->setVelY(BULLET_SPEED * sin(M_PI * 7 / 8));
    }
    else if (status.action == Action::AIM_DOWN_RIGHT_WHILE_WALKING)
    {
        newBullet->setPos(x + 69 - BULLET_SPEED * cos(M_PI * 1 / 8), y - BULLET_SPEED * sin(M_PI * 7 / 8) + 67);
        newBullet->setVelX(BULLET_SPEED * cos(M_PI * 1 / 8));
        newBullet->setVelY(BULLET_SPEED * sin(M_PI * 7 / 8));
    }
    else if (status.action == Action::AIM_UP_LEFT_WHILE_WALKING)
    {
        newBullet->setPos(x + BULLET_SPEED * cos(M_PI * 1 / 8), y + BULLET_SPEED * sin(M_PI * 7 / 8) + 3);
        newBullet->setVelX(-BULLET_SPEED * cos(M_PI * 1 / 8));
        newBullet->setVelY(-BULLET_SPEED * sin(M_PI * 7 / 8));
    }
    else if (status.action == Action::AIM_UP_RIGHT_WHILE_WALKING)
    {
        newBullet->setPos(x + 63 - BULLET_SPEED * cos(M_PI * 1 / 8), y + BULLET_SPEED * sin(M_PI * 7 / 8) + 3);
        newBullet->setVelX(BULLET_SPEED * cos(M_PI * 1 / 8));
        newBullet->setVelY(-BULLET_SPEED * sin(M_PI * 7 / 8));
    }
    else if (status.action == Action::AIM_UP_LEFT)
    {
        newBullet->setPos(x + 6, y + BULLET_SPEED);
        newBullet->setVelX(0);
        newBullet->setVelY(-BULLET_SPEED);
    }
    else if (status.action == Action::AIM_UP_RIGHT)
    {
        newBullet->setPos(x + 26, y + BULLET_SPEED);
        newBullet->setVelX(0);
        newBullet->setVelY(-BULLET_SPEED);
    }
    else if (status.action == Action::LAYING_DOWN_LEFT)
    {
        newBullet->setPos(x + BULLET_SPEED, y + 19);
        newBullet->setVelX(-BULLET_SPEED);
        newBullet->setVelY(0);
    }
    else if (status.action == Action::LAYING_DOWN_RIGHT)
    {
        newBullet->setPos(x + 102 - BULLET_SPEED, y + 19);
        newBullet->setVelX(BULLET_SPEED);
        newBullet->setVelY(0);
    }
    else if (status.action == Action::JUMPING)
    {
        newBullet->setPos(x + 30 - BULLET_SPEED, y + 30);
        if (direction.left == true)
            newBullet->setVelX(-BULLET_SPEED);
        if (direction.right == true)
            newBullet->setVelX(BULLET_SPEED);
        // Xử lí sau
    }
    else if (status.action == Action::AIM_RIGHT_WHILE_WALKING)
    {
        newBullet->setPos(x + 81 - BULLET_SPEED, y + 30);
        newBullet->setVelX(BULLET_SPEED);
        newBullet->setVelY(0);
        // std::cout << 100;
    }
    else if (status.action == Action::AIM_LEFT_WHILE_WALKING)
    {
        newBullet->setPos(x + BULLET_SPEED, y + 30);
        newBullet->setVelX(-BULLET_SPEED);
        newBullet->setVelY(0);
    }
    else
        newBullet->setPos(x, y);

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
