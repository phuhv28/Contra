#include "Constants.h"
#include "Object.h"
#include "Player.h"
#include "Map.h"

SDL_Window *window;
SDL_Renderer *renderer;
Object backGround;
Player player;
GameMap map;
SDL_Rect camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

bool init();
void renderGamePlay();
void close();

int main(int argc, char *argv[])
{

    // Start up SDL and create window
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

            map.loadMap("map/map.txt");

            player.loadIMG("res/right.png", renderer);

            SDL_Event e;

            while (!quit)
            {
                while (SDL_PollEvent(&e) != 0)
                {
                    if (e.type == SDL_QUIT)
                    {
                        quit = true;
                    }

                    player.handleInput(e, renderer);
                }

                renderGamePlay();
            }
        }
    }

    close();

    return 0;
}

bool init()
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
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
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

void renderGamePlay()
{
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(renderer);

    camera.x = (player.getX() + player.getFrameW() / 2) - SCREEN_WIDTH / 2;
    camera.y = (player.getY() + player.getFrameH() / 2) - SCREEN_HEIGHT / 2;

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

    backGround.render(renderer, &camera);

    player.action(map.getMap());

    player.show(renderer, &camera);

    SDL_RenderPresent(renderer);
}

void close()
{
    backGround.free();
    SDL_DestroyWindow(window);
    window = NULL;
    SDL_DestroyRenderer(renderer);
    renderer = NULL;
}
