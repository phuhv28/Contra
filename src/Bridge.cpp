#include "Bridge.h"

Bridge::Bridge()
{
    isExploding = false;
    curFrame = 0;
    aCurFrame = 0;
    for (int i = 0; i < 5; i++)
    {
        frameClip[i].x = 0;
        frameClip[i].y = i * 96;
        frameClip[i].w = 384;
        frameClip[i].h = 96;
    }
}

Bridge::~Bridge()
{
}

void Bridge::show()
{
    SDL_Rect renderQuad = {x - Object::camera.x, y - Object::camera.y, 384, 96};
    SDL_RenderCopy(renderer, texture, &frameClip[curFrame], &renderQuad);

    if (curFrame >= 1 && aCurFrame < 99)
    {
    explosion.loadIMG("res/img/explosion.png");
    explosion.setPos(x + (curFrame - 1) * TILE_SIZE, y);
    explosion.render();
    }

}

void Bridge::action(int playerX, GameMap &gameMap)
{
    if (x - playerX <= 100 && curFrame == 0)
        isExploding = true;

    if (isExploding)
    {
        gameMap.changeMap((x + curFrame % 4 * TILE_SIZE) / TILE_SIZE, y / TILE_SIZE, 0);
        std::cout << 1 << " ";

        if (curFrame < 5)
            aCurFrame++;
        curFrame = aCurFrame / 20;
        if (aCurFrame >= 99)
            isExploding = false;

        // std::cout << aCurFrame << " ";
    }
}
