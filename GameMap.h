#ifndef GAMEMAP_H_INCLUDED
#define GAMEMAP_H_INCLUDED

#include <SDL.h>
#include <vector>
#include "player.h"
#include "enemy.h"
#include "commonfunction.h"
#include "PowerUp.h"
#include <SDL_mixer.h>

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
    // SỬA HÀM KHỞI TẠO: Thêm tham số cho Texture và Sound của vụ nổ
    Game(SDL_Renderer* renderer, SDL_Texture* roadTex,
        const std::vector<SDL_Texture*>& enemyTexList,
        SDL_Texture* heartTex, SDL_Texture* shieldTex,
        SDL_Texture* healTex,
        SDL_Texture* explodeTex, Mix_Chunk* explodeSFX);

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

    // BIẾN MỚI CHO TÍNH NĂNG TĂNG TỐC <--- THÊM
    int currentSpeed;
    Uint32 lastSpeedIncreaseTime;

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

    SDL_Texture* explodeTexture;
    Mix_Chunk* explodeSound;
    bool isExploding;
    SDL_Rect playerExplodeRect;
    Uint32 explodeStartTime;
};

#endif // GAMEMAP_H_INCLUDED
