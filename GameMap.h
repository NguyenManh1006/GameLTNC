#ifndef GAMEMAP_H_INCLUDED
#define GAMEMAP_H_INCLUDED

#include <SDL.h>
#include <vector>
#include "player.h"
#include "enemy.h"
#include "commonfunction.h"
#include "PowerUp.h"

enum class GameState {
    PLAYING,
    GAME_OVER
};

enum class GameOverOption {
    RETRY,
    MENU,
    NONE
};

class Game {
public:
    Game(SDL_Renderer* renderer, SDL_Texture* roadTex,
        const std::vector<SDL_Texture*>& enemyTexList,
        SDL_Texture* heartTex, SDL_Texture* shieldTex,
        SDL_Texture* healTex);

    ~Game();

    void SetPlayer(Player* p);
    void Update();
    void Render();
    void HandleEvent(SDL_Event e);
    bool IsRunning() const { return isRunning; }
    void ResetGame();
    GameState GetState() const { return state; }

    GameOverOption ShowGameOver(SDL_Renderer* renderer);

private:
    SDL_Renderer* renderer;
    SDL_Texture* roadTexture;
    std::vector<SDL_Texture*> enemyTextures;

    Player* player;
    std::vector<Enemy*> enemies;

    std::vector<PowerUp*> powerups;
    SDL_Texture* shieldTexture;
    SDL_Texture* healTexture;

    int frameCount;
    int backgroundY;

    int hp;
    int maxHp;
    bool invincible;
    Uint32 invincibleStart;

    bool hasShield;
    Uint32 shieldStart;
    Uint32 lastPowerUpSpawnTime;
    SDL_Texture* heartTexture;

    int score;
    int highscore;
    Uint32 lastScoreTime;

    void LoadHighscore();
    void SaveHighscore();

    bool isRunning;

    GameState state;
    void RenderGameOver();
};

#endif // GAMEMAP_H_INCLUDED
