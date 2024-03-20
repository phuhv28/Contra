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
    status.action = Action::JUMPING;
    lay = false;
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
        switch (status.action)
        {
        // case Action::JUMPING:
        //     numFrame = 3;
        //     loadIMG("res/jump.png", renderer);
        //     break;
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

    curFrame = aCurFrame / SLOWMOTION_ANIMATION_RATE;

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
        case SDLK_UP:
            inputQueue.push_back(Input::UP);
            break;
        case SDLK_DOWN:
            inputQueue.push_back(Input::DOWN);
            break;
        case SDLK_x:
            inputQueue.push_back(Input::X);
            break;
        case SDLK_z:
            // createBullet(renderer);
            // status.isFiring = true;
            inputQueue.push_back(Input::Z);
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
        case SDLK_x:
            for (int i = 0; i < (int)inputQueue.size(); i++)
            {
                if (inputQueue[i] == Input::X)
                    inputQueue.erase(inputQueue.begin() + i);
            }
            break;
        case SDLK_z:
            for (int i = 0; i < (int)inputQueue.size(); i++)
            {
                if (inputQueue[i] == Input::Z)
                    inputQueue.erase(inputQueue.begin() + i);
            }
            break;
        }
    }

    // std::cout << inputQueue.size() << std::endl;

    if ((int)inputQueue.size() != 0)
    {
        if (inputQueue[inputQueue.size() - 1] == Input::X)
        {
            if (status.onGround == true)
            {
                bool flag = false;
                for (int i = (int)inputQueue.size() - 2; i >= 0; i--)
                {
                    if (inputQueue[i] == Input::DOWN)
                    {
                        status.isFalling = true;
                        status.onGround = false;
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
            inputQueue.pop_back();
        }
        else
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
                        VelX = SPEED_X;
                        direction.right = true;
                        direction.left = false;
                        status.action = Action::WALKING_RIGHT;
                    }
                    else
                    {
                        VelX = -SPEED_X;
                        direction.left = true;
                        direction.right = false;
                        status.action = Action::WALKING_LEFT;
                    }
                }
                else if ((int)inputQueue_.size() == 2)
                {
                    VelX = SPEED_X;
                    direction.right = true;
                    direction.left = false;
                    status.action = Action::WALKING_RIGHT;
                }
            }
            else if ((int)inputQueue.size() == 2)
            {
                if ((inputQueue[0] == Input::DOWN && inputQueue[1] == Input::LEFT) || (inputQueue[0] == Input::LEFT && inputQueue[1] == Input::DOWN))
                {
                    VelX = -SPEED_X;
                    direction.right = false;
                    direction.left = true;
                    status.action = Action::AIM_DOWN_LEFT_WHILE_WALKING;
                }
                if ((inputQueue[0] == Input::DOWN && inputQueue[1] == Input::RIGHT) || (inputQueue[0] == Input::RIGHT && inputQueue[1] == Input::DOWN))
                {
                    VelX = SPEED_X;
                    direction.right = true;
                    direction.left = false;
                    status.action = Action::AIM_DOWN_RIGHT_WHILE_WALKING;
                }
                if ((inputQueue[0] == Input::UP && inputQueue[1] == Input::LEFT) || (inputQueue[0] == Input::LEFT && inputQueue[1] == Input::UP))
                {
                    VelX = -SPEED_X;
                    direction.right = false;
                    direction.left = true;
                    status.action = Action::AIM_UP_LEFT_WHILE_WALKING;
                }
                if ((inputQueue[0] == Input::UP && inputQueue[1] == Input::RIGHT) || (inputQueue[0] == Input::RIGHT && inputQueue[1] == Input::UP))
                {
                    VelX = SPEED_X;
                    direction.right = true;
                    direction.left = false;
                    status.action = Action::AIM_UP_RIGHT_WHILE_WALKING;
                }
            }
            else
            {
                if (inputQueue[0] == Input::DOWN)
                {
                    if (status.onGround == true && !lay)
                    {
                        lay  = true;
                        if (direction.left == true)
                            status.action = Action::LAYING_DOWN_LEFT;
                        else if (direction.right == true)
                        {
                            status.action = Action::LAYING_DOWN_RIGHT;
                        }
                        VelX = 0;
                        dx = 40;
                    }
                    else {
                        dx = 0;
                        lay = false;
                    }
                }
                else if (inputQueue[0] == Input::UP)
                {
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
                    VelX = SPEED_X;
                    direction.right = true;
                    direction.left = false;
                    status.action = Action::WALKING_RIGHT;
                }
                else if (inputQueue[0] == Input::LEFT)
                {
                    VelX = -SPEED_X;
                    direction.right = false;
                    direction.left = true;
                    status.action = Action::WALKING_LEFT;
                }
            }
        }
    }
    else
    {
        VelX = 0;
    }
}

void Player::action(Map map)
{
    // std::cout << (int) status.action;
    if (VelX == 0 && status.onGround == true && inputQueue.empty())
    {
        if (direction.right == true)
            status.action = Action::STANDING_RIGHT;
        if (direction.left == true)
            status.action = Action::STANDING_LEFT;
    }

    int x1 = x + VelX;
    int x2 = x1 + 40;
    int y1 = y + VelY;
    int y2 = y1 + frameClip[curFrame].h;
    if ((map.tile[y2 / TILE_SIZE][x1 / TILE_SIZE] == 1 ||
         map.tile[y2 / TILE_SIZE][x2 / TILE_SIZE] == 1) &&
        VelY > 0 && status.isFalling == false && status.onGround == false)
    {
        y = y2 / TILE_SIZE * TILE_SIZE - frameClip[curFrame].h;
        VelY = 0;
        status.onGround = true;
    }

    if (status.isFalling == true)
    {
        status.isFalling = false;
        direction.down = false;
    }
    y += dx;
    if(lay == true) dx = 0;
    

    // Fall when go out of block 1
    if (VelY == 0 && status.onGround == true &&
        (map.tile[y2 / TILE_SIZE][x1 / TILE_SIZE] != 1 && map.tile[y2 / TILE_SIZE][x2 / TILE_SIZE] != 1))
    {
        VelY = GRAVITY;
    }

    x += VelX;
    y += VelY;

    if (status.onGround == false)
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
