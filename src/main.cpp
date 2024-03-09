#include "Constants.h"
#include "Object.h"
#include "Player.h"
#include "Map.h"

SDL_Window *window;
SDL_Renderer *renderer;
Object backGround;

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

void close()
{
    backGround.free();
    SDL_DestroyWindow(window);
    window = NULL;
    SDL_DestroyRenderer(renderer);
    renderer = NULL;
}

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

            GameMap map;
            map.loadMap("map/map.txt");

            Player player;
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
                SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderClear(renderer);

                backGround.render(renderer);

                player.show(renderer);

                SDL_RenderPresent(renderer);
            }
        }
    }

    close();

    return 0;
}
