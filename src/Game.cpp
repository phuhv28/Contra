#include "Game.h"

Game::Game()
{
    window = nullptr;
    renderer = nullptr;
    camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
}

Game::~Game()
{
}

bool Game::init()
{
    bool success = true;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        std::cout << "SDL could not initialize! SDL Error:\n"
                  << SDL_GetError();
        success = false;
    }
    else
    {
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
        {
            std::cout << "Warning: Linear texture filtering not enabled!";
        }

        window = SDL_CreateWindow("Contra", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (window == NULL)
        {
            std::cout << "Window could not be created! SDL Error: \n"
                      << SDL_GetError();
            success = false;
        }
        else
        {
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            if (renderer == NULL)
            {
                std::cout << "Renderer could not be created! SDL Error:\n"
                          << SDL_GetError();
                success = false;
            }
            else
            {
                SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags))
                {
                    std::cout << "SDL_image could not initialize! SDL_image Error:\n"
                              << IMG_GetError();
                    success = false;
                }
                // Initialize SDL_mixer
                if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
                {
                    printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
                }
            }
        }
    }

    return success;
}

void Game::setCamera()
{
    int tempX = (player.getX() + player.getFrameW() / 2) - SCREEN_WIDTH / 2;
    if (tempX > camera.x)
        camera.x = tempX;
    camera.y = 0;

    if (camera.x < 0)
    {
        camera.x = 0;
    }
    if (camera.y < 0)
    {
        camera.y = 0;
    }
    if (camera.x > MAX_MAP_X * TILE_SIZE - camera.w)
    {
        camera.x = MAX_MAP_X * TILE_SIZE - camera.w;
    }
    if (camera.y > MAX_MAP_Y * TILE_SIZE - camera.h)
    {
        camera.y = MAX_MAP_Y * TILE_SIZE - camera.h;
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
    pEnemy->setX(864);
    pEnemy->setY(207);
    enemyList1.push_back(pEnemy);

    pEnemy = new Enemy1();
    pEnemy->setX(960);
    pEnemy->setY(207);
    enemyList1.push_back(pEnemy);

    return enemyList1;
}

std::vector<Enemy2 *> Game::createEnemies2()
{
    std::vector<Enemy2 *> enemyList2;

    Enemy2 *pEnemy = new Enemy2();

    pEnemy->setX(960);
    pEnemy->setY(474);
    enemyList2.push_back(pEnemy);

    return enemyList2;
}

void Game::removeEnemy(int index)
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

void Game::handleEnemy()
{
    for (int i = 0; i < enemyList1.size(); i++)
    {
        if (enemyList1[i]->getX() < camera.x || enemyList1[i]->getY() < 0 || enemyList1[i]->getY() > SCREEN_HEIGHT)
            removeEnemy(i);
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
                        removeEnemy(i);
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
    SDL_Texture *texture = IMG_LoadTexture(renderer, "res/splashscreen.png");

    SDL_Rect icon = {0, 0, 36, 20};
    SDL_Texture *iconTexture = IMG_LoadTexture(renderer, "res/icon1.png");

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

    gameOver = Mix_LoadMUS("res/game_over.wav");
    Mix_PlayMusic(gameOver, 0);
    SDL_Texture *texture = NULL;

    SDL_Surface *loadedSurface = IMG_Load("res/gameover.png");
    texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);

    SDL_FreeSurface(loadedSurface);

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
    player.handleInputQueue(e, renderer);
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(renderer);

    for (int i = 0; i < enemyList1.size(); i++)
    {
        Enemy1 *pEnemy = enemyList1[i];
        if (pEnemy != NULL)
            pEnemy->action(map.getMap(), camera);
    }
    for (int i = 0; i < enemyList2.size(); i++)
    {
        Enemy2 *pEnemy = enemyList2[i];
        if (pEnemy != NULL)
            pEnemy->action(map.getMap(), camera, player.getX() + player.getFrameW() / 2, player.getY() + player.getFrameH() / 2, renderer);
    }
    // std::cout << enemyList2.size() << " ";

    player.action(map.getMap());
    handleCol();

    setCamera();
    backGround.render(renderer, &camera);
    player.handleBullet(renderer);
    for (int i = 0; i < enemyList2.size(); i++)
    {
        enemyList2[i]->handleBullet(renderer, camera);
    }

    handleEnemy();
    player.show(renderer);

    for (int i = 0; i < enemyList1.size(); i++)
    {
        Enemy1 *pEnemy = enemyList1[i];
        if (pEnemy != NULL)
            pEnemy->show(renderer, camera);
    }
    for (int i = 0; i < enemyList2.size(); i++)
    {
        Enemy2 *pEnemy = enemyList2[i];
        if (pEnemy != NULL)
        {
            pEnemy->show(renderer, camera);
            // std::cout << 100;
        }
    }

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
    if (!init())
    {
        std::cout << "Failed to initialize!\n";
    }
    else
    {
        if (!backGround.loadIMG("res/background.png", renderer))
        {
            std::cout << "Failed to load media!\n";
        }
        else
        {
            bool quit = false;

            player.setCam(camera);
            enemyList1 = createEnemies1();
            enemyList2 = createEnemies2();
            title = Mix_LoadWAV("res/title.wav");
            fireSound = Mix_LoadWAV("res/rifle.wav");

            if (title == NULL)
            {
                printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
            }
            gMusic = Mix_LoadMUS("res/music.wav");

            map.loadMap("map/map.txt");
            player.loadIMG("res/standingR.png", renderer);

            renderSplashScreen();

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

                    player.getInput(e, renderer, fireSound);
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

            renderGameOver();
        }
    }

    close();
}
