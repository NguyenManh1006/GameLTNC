#ifndef GAMEMAP_H_INCLUDED
#define GAMEMAP_H_INCLUDED

#include <SDL.h>
#include <vector>
#include "player.h"
#include "enemy.h"
#include "commonfunction.h"

class Game {
public:
    Game(SDL_Renderer* renderer, SDL_Texture* roadTex, SDL_Texture* enemyTex);
    ~Game();

    void SetPlayer(Player* p);
    void Update();
    void Render();

private:
    SDL_Renderer* renderer;
    SDL_Texture* roadTexture;
    SDL_Texture* enemyTexture;

    Player* player;
    std::vector<Enemy*> enemies;

    int frameCount;
    int backgroundY;
};

#endif // GAMEMAP_H_INCLUDED
