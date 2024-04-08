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

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
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

std::vector<Enemy *> Game::createEnemies()
{
    std::vector<Enemy *> enemyList;
    Enemy *enemy = new Enemy[10];

    Enemy *pEnemy = (enemy);

    pEnemy->setX(768);
    pEnemy->setY(207);
    enemyList.push_back(pEnemy);

    pEnemy = enemy + 1;
    pEnemy->setX(864);
    pEnemy->setY(207);
    enemyList.push_back(pEnemy);

    pEnemy = enemy + 2;
    pEnemy->setX(960);
    pEnemy->setY(207);
    enemyList.push_back(pEnemy);

    return enemyList;
}


//Copy Lazyfoo
bool Game::checkCol(const SDL_Rect& a, const SDL_Rect& b)
{
    //The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    //Calculate the sides of rect A
    leftA = a.x;
    rightA = a.x + a.w;
    topA = a.y;
    bottomA = a.y + a.h;

    //Calculate the sides of rect B
    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;
    //If any of the sides from A are outside of B
    if( bottomA <= topB )
    {
        return false;
    }

    if( topA >= bottomB )
    {
        return false;
    }

    if( rightA <= leftB )
    {
        return false;
    }

    if( leftA >= rightB )
    {
        return false;
    }

    //If none of the sides from A are outside B
    return true;
}

void Game::handleCol()
{
    for (int i = 0; i < enemyList.size(); i++)
    {
        if (enemyList[i] != NULL)
        {
            SDL_Rect a = {enemyList[i]->getX(), enemyList[i]->getY(), 68, 96};
            // std:: cout << a.x << " " << a.y << " " << a.w << " " << a.h << std::endl;
            SDL_Rect b = {player.getX(), player.getY(), player.getFrameW(), player.getFrameH()};
            // std:: cout << b.x << " " << b.y << " " << b.w << " " << b.h << std::endl;
            if (checkCol(a, b))
            {
                if(!player.isDead())
                    player.setDied();
            }
        }
    }
}

void Game::renderGamePlay()
{
    player.handleInputQueue(e, renderer);
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(renderer);

    for (int i = 0; i < enemyList.size(); i++)
    {
        Enemy *pEnemy = enemyList[i];
        if (pEnemy != NULL)
            pEnemy->action(map.getMap(), camera);
    }

    player.action(map.getMap());
    handleCol();

    setCamera();
    backGround.render(renderer, &camera);
    player.handleBullet(renderer);
    player.show(renderer);

    for (int i = 0; i < enemyList.size(); i++)
    {
        Enemy *pEnemy = enemyList[i];
        if (pEnemy != NULL)
            pEnemy->show(renderer, camera);
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
            enemyList = createEnemies();

            map.loadMap("map/map.txt");
            player.loadIMG("res/standingR.png", renderer);

            while (!quit)
            {
                auto start = CLOCK_NOW();
                while (SDL_PollEvent(&e) != 0)
                {
                    if (e.type == SDL_QUIT)
                    {
                        quit = true;
                    }

                    player.getInput(e, renderer);
                }

                auto end = CLOCK_NOW();

                ElapsedTime elapsedTime = end - start;
                // std::cout << elapsedTime.count() << " ";

                if (elapsedTime.count() < SCREEN_TICKS_PER_FRAME)
                {
                    SDL_Delay(SCREEN_TICKS_PER_FRAME - elapsedTime.count());
                }
                renderGamePlay();
            }
        }
    }

    close();
}
