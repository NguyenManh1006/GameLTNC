#ifndef GAMEMAP_H_INCLUDED
#define GAMEMAP_H_INCLUDED

#include <SDL.h>
#include <vector>
#include "player.h"
#include "enemy.h"
#include "commonfunction.h"

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
    Game(SDL_Renderer* renderer, SDL_Texture* roadTex, const std::vector<SDL_Texture*>& enemyTexList, SDL_Texture* heartTex);

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

    int frameCount;
    int backgroundY;

    int hp;
    bool invincible;
    Uint32 invincibleStart;
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
