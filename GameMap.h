#ifndef GAMEMAP_H_INCLUDED
#define GAMEMAP_H_INCLUDED

#include <SDL.h>
#include <vector>
#include "player.h"
#include "enemy.h"
#include "commonfunction.h"

class Game {
public:
    Game(SDL_Renderer* renderer, SDL_Texture* roadTex, const std::vector<SDL_Texture*>& enemyTexList, SDL_Texture* heartTex);

    ~Game();

    void SetPlayer(Player* p);
    void Update();
    void Render();

private:
    SDL_Renderer* renderer;
    SDL_Texture* roadTexture;
    std::vector<SDL_Texture*> enemyTextures;

    Player* player;
    std::vector<Enemy*> enemies;

    int frameCount;
    int backgroundY;

    int hp;
    bool invincible;
    Uint32 invincibleStart;
    SDL_Texture* heartTexture;
};

#endif // GAMEMAP_H_INCLUDED
