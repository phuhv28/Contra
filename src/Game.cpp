#include "Game.h"

Game::Game(SDL_Renderer *renderer, SDL_Window *window)
{
    this->renderer = renderer;
    this->window = window;
    Object::renderer = this->renderer;
    backGround.loadIMG("res/img/background.png");
    enemyList1 = createEnemies1();
    enemyList2 = createEnemies2();
    enemyList3 = createEnemies3();
    title = Mix_LoadWAV("res/sound/title.wav");
    fireSound = Mix_LoadWAV("res/sound/rifle.wav");
    gMusic = Mix_LoadMUS("res/sound/music.wav");
    map.loadMap("map/map.txt");
    player.loadIMG("res/img/standingR.png");
    gameOver = Mix_LoadMUS("res/sound/game_over.wav");
    bridge[0].loadIMG("res/img/bridge.png");
    bridge[1].loadIMG("res/img/bridge.png");

    bridge[0].setX(2304);
    bridge[0].setY(288);
    bridge[1].setX(3168);
    bridge[1].setY(288);
}

Game::~Game()
{
}

void Game::setCamera()
{
    int tempX = (player.getX() + player.getFrameW() / 2) - SCREEN_WIDTH / 2;
    if (tempX > Object::camera.x)
        Object::camera.x = tempX;
    Object::camera.y = 0;

    if (Object::camera.x < 0)
    {
        Object::camera.x = 0;
    }
    if (Object::camera.y < 0)
    {
        Object::camera.y = 0;
    }
    if (Object::camera.x > MAX_MAP_X * TILE_SIZE - Object::camera.w)
    {
        Object::camera.x = MAX_MAP_X * TILE_SIZE - Object::camera.w;
    }
    if (Object::camera.y > MAX_MAP_Y * TILE_SIZE - Object::camera.h)
    {
        Object::camera.y = MAX_MAP_Y * TILE_SIZE - Object::camera.h;
    }
}

std::vector<Enemy1 *> Game::createEnemies1()
{
    std::vector<Enemy1 *> enemyList1;

    Enemy1 *pEnemy = new Enemy1();

    pEnemy->setX(768);
    pEnemy->setY(207);
    enemyList1.push_back(pEnemy);

    pEnemy = new Enemy1();
    pEnemy->setX(960);
    pEnemy->setY(207);
    enemyList1.push_back(pEnemy);

    pEnemy = new Enemy1();
    pEnemy->setX(1088);
    pEnemy->setY(207);
    enemyList1.push_back(pEnemy);

    pEnemy = new Enemy1();
    pEnemy->setX(1632);
    pEnemy->setY(207);
    enemyList1.push_back(pEnemy);

    pEnemy = new Enemy1();
    pEnemy->setX(1824);
    pEnemy->setY(207);
    enemyList1.push_back(pEnemy);

    pEnemy = new Enemy1();
    pEnemy->setX(2464);
    pEnemy->setY(207);
    enemyList1.push_back(pEnemy);

    pEnemy = new Enemy1();
    pEnemy->setX(2944);
    pEnemy->setY(207);
    enemyList1.push_back(pEnemy);
    
    pEnemy = new Enemy1();
    pEnemy->setX(3264);
    pEnemy->setY(207);
    enemyList1.push_back(pEnemy);

    pEnemy = new Enemy1();
    pEnemy->setX(3872);
    pEnemy->setY(207);
    enemyList1.push_back(pEnemy);

    pEnemy = new Enemy1();
    pEnemy->setX(4064);
    pEnemy->setY(207);
    enemyList1.push_back(pEnemy);
    return enemyList1;
}

std::vector<Enemy2 *> Game::createEnemies2()
{
    std::vector<Enemy2 *> enemyList2;

    Enemy2 *pEnemy = new Enemy2();

    pEnemy->setX(928);
    pEnemy->setY(474);
    enemyList2.push_back(pEnemy);

    pEnemy = new Enemy2();
    pEnemy->setX(1888);
    pEnemy->setY(474);
    enemyList2.push_back(pEnemy);
    
    pEnemy = new Enemy2();
    pEnemy->setX(3808);
    pEnemy->setY(186);
    enemyList2.push_back(pEnemy);

    return enemyList2;
}

std::vector<Enemy3 *> Game::createEnemies3()
{
    std::vector<Enemy3 *> enemyList3;

    Enemy3 *pEnemy = new Enemy3();

    pEnemy->setX(3744);
    pEnemy->setY(384);
    enemyList3.push_back(pEnemy);


    return enemyList3;
}

void Game::removeEnemy(int enemyType, int index)
{
    if (enemyType == 1)
    {
        int size = enemyList1.size();
        if (size > 0 && index < size)
        {
            if (enemyList1[index] != NULL)
            {
                enemyList1[index]->free();
                delete enemyList1[index];
                enemyList1.erase(enemyList1.begin() + index);
            }
        }
    }
    else if (enemyType == 2)
    {
        int size = enemyList2.size();
        if (size > 0 && index < size)
        {
            if (enemyList2[index] != NULL)
            {
                enemyList2[index]->free();
                delete enemyList2[index];
                enemyList2.erase(enemyList2.begin() + index);
            }
        }
    }
}

void Game::handleEnemy()
{
    for (int i = 0; i < enemyList1.size(); i++)
    {
        if (enemyList1[i]->getX() < Object::camera.x || enemyList1[i]->getY() < 0 || enemyList1[i]->getY() > SCREEN_HEIGHT)
            removeEnemy(1, i);
    }
}

// Copy Lazyfoo
bool Game::checkCol(const SDL_Rect &a, const SDL_Rect &b)
{
    // The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    // Calculate the sides of rect A
    leftA = a.x;
    rightA = a.x + a.w;
    topA = a.y;
    bottomA = a.y + a.h;

    // Calculate the sides of rect B
    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;
    // If any of the sides from A are outside of B
    if (bottomA <= topB)
    {
        return false;
    }

    if (topA >= bottomB)
    {
        return false;
    }

    if (rightA <= leftB)
    {
        return false;
    }

    if (leftA >= rightB)
    {
        return false;
    }

    // If none of the sides from A are outside B
    return true;
}

void Game::handleCol()
{
    // Collision player with enemies

    for (int i = 0; i < enemyList1.size(); i++)
    {
        if (enemyList1[i] != NULL)
        {
            SDL_Rect a = {enemyList1[i]->getX(), enemyList1[i]->getY(), 68, 96};
            // std:: cout << a.x << " " << a.y << " " << a.w << " " << a.h << std::endl;
            SDL_Rect b = {player.getX(), player.getY(), player.getFrameW(), player.getFrameH()};
            // std:: cout << b.x << " " << b.y << " " << b.w << " " << b.h << std::endl;
            if (checkCol(a, b))
            {
                if (!player.isDead())
                    player.setDied();
            }
        }
    }

    for (int i = 0; i < enemyList2.size(); i++)
    {
        if (enemyList2[i] != NULL)
        {
            SDL_Rect a = {enemyList2[i]->getX(), enemyList2[i]->getY(), 68, 96};
            // std:: cout << a.x << " " << a.y << " " << a.w << " " << a.h << std::endl;
            SDL_Rect b = {player.getX(), player.getY(), player.getFrameW(), player.getFrameH()};
            // std:: cout << b.x << " " << b.y << " " << b.w << " " << b.h << std::endl;
            if (checkCol(a, b))
            {
                if (!player.isDead())
                    player.setDied();
            }
        }
    }

    // Collision player's bullet with enenmies

    std::vector<Bullet *> bulletList = player.getBullet();

    for (int i = 0; i < enemyList1.size(); i++)
    {
        if (enemyList1[i] != NULL)
        {
            for (int j = 0; j < bulletList.size(); j++)
            {
                if (bulletList[j] != NULL)
                {
                    SDL_Rect a = {bulletList[j]->getX(), bulletList[j]->getY(), 10, 10};
                    SDL_Rect b = {enemyList1[i]->getX(), enemyList1[i]->getY(), 68, 96};
                    if (checkCol(a, b))
                    {
                        removeEnemy(1, i);
                        player.removeBullet(j);
                    }
                }
            }
        }
    }

    for (int i = 0; i < enemyList2.size(); i++)
    {
        if (enemyList2[i] != NULL)
        {
            for (int j = 0; j < bulletList.size(); j++)
            {
                if (bulletList[j] != NULL)
                {
                    SDL_Rect a = {bulletList[j]->getX(), bulletList[j]->getY(), 10, 10};
                    SDL_Rect b = {enemyList2[i]->getX(), enemyList2[i]->getY(), 68, 96};
                    if (checkCol(a, b))
                    {
                        removeEnemy(2, i);
                        player.removeBullet(j);
                    }
                }
            }
        }
    }
}

void Game::renderSplashScreen()
{
    Mix_PlayChannel(-1, title, 0);
    SDL_Texture *texture = IMG_LoadTexture(renderer, "res/img/splashscreen.png");

    SDL_Rect icon = {0, 0, 36, 20};
    SDL_Texture *iconTexture = IMG_LoadTexture(renderer, "res/img/icon1.png");

    bool quit = false;
    int option = 0;
    while (!quit)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
                quit = true;
            if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
            {
                switch (e.key.keysym.sym)
                {
                case SDLK_UP:
                    option--;
                    option = (option + 2) % 2;
                    break;
                case SDLK_DOWN:
                    option++;
                    option = (option + 2) % 2;
                    break;
                case SDLK_RETURN:
                    quit = true;
                    if (option == 0)
                    {
                    }
                    else
                    {
                    }

                    break;
                }
            }

            if (option == 0)
            {
                icon.x = 156;
                icon.y = 435;
            }
            else
            {
                icon.x = 156;
                icon.y = 480;
            }

            SDL_RenderCopy(renderer, texture, NULL, NULL);

            SDL_RenderCopy(renderer, iconTexture, NULL, &icon);
            SDL_RenderPresent(renderer);
        }
    }
}

void Game::renderGameOver()
{
    if (Mix_PlayingMusic() == 1)
    {
        Mix_PauseMusic();
    }

    Mix_PlayMusic(gameOver, 0);
    SDL_Texture *texture = IMG_LoadTexture(renderer, "res/img/gameover.png");

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
    bool quit = false;
    while (!quit)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
        }
    }
}

void Game::renderGamePlay()
{
    player.handleInputQueue(e);
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(renderer);

    for (int i = 0; i < enemyList1.size(); i++)
    {
        Enemy1 *pEnemy = enemyList1[i];
        if (pEnemy != NULL)
            pEnemy->action(map.getMap());
    }
    for (int i = 0; i < enemyList2.size(); i++)
    {
        Enemy2 *pEnemy = enemyList2[i];
        if (pEnemy != NULL)
            pEnemy->action(map.getMap(), player.getX() + player.getFrameW() / 2, player.getY() + player.getFrameH() / 2);
    }
    for (int i = 0; i < enemyList3.size(); i++)
    {
        Enemy3 *pEnemy = enemyList3[i];
        if (pEnemy != NULL)
            pEnemy->action(map.getMap(), player.getX() + player.getFrameW() / 2, player.getY() + player.getFrameH() / 2);
    }

    player.action(map.getMap());
    bridge[0].action(player.getX(), map);
    bridge[1].action(player.getX(), map);
    handleCol();

    setCamera();
    backGround.render(&Object::camera);
    player.handleBullet();
    for (int i = 0; i < enemyList2.size(); i++)
    {
        enemyList2[i]->handleBullet();
    }
    for (int i = 0; i < enemyList3.size(); i++)
    {
        enemyList3[i]->handleBullet();
    }

    handleEnemy();
    bridge[0].show();
    bridge[1].show();

    for (int i = 0; i < enemyList1.size(); i++)
    {
        Enemy1 *pEnemy = enemyList1[i];
        if (pEnemy != NULL)
            pEnemy->show();
    }
    for (int i = 0; i < enemyList2.size(); i++)
    {
        Enemy2 *pEnemy = enemyList2[i];
        if (pEnemy != NULL)
        {
            pEnemy->show();
        }
    }
    for (int i = 0; i < enemyList3.size(); i++)
    {
        Enemy3 *pEnemy = enemyList3[i];
        if (pEnemy != NULL)
        {
            pEnemy->show();
        }
    }

    player.show();


    SDL_RenderPresent(renderer);
}

void Game::close()
{
    backGround.free();
    SDL_DestroyWindow(window);
    window = NULL;
    SDL_DestroyRenderer(renderer);
    renderer = NULL;
    Mix_FreeChunk(title);
    title = NULL;
}

void Game::run()
{
    bool quit = false;
    // renderSplashScreen();

    Mix_Pause(-1);
    Mix_FreeChunk(title);
    Mix_PlayMusic(gMusic, -1);

    
    while (!quit)
    {
        auto start = CLOCK_NOW();
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }

            player.getInput(e, fireSound);
        }

        auto end = CLOCK_NOW();

        ElapsedTime elapsedTime = end - start;
        // std::cout << elapsedTime.count() << " ";

        if (elapsedTime.count() < SCREEN_TICKS_PER_FRAME)
        {
            SDL_Delay(SCREEN_TICKS_PER_FRAME - elapsedTime.count());
        }
        renderGamePlay();
        if (player.isDead())
            break;
    }

    // renderGameOver();

    close();
}
