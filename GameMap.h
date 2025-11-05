#ifndef GAMEMAP_H_INCLUDED
#define GAMEMAP_H_INCLUDED

#include <SDL.h>
#include <vector>
#include "player.h"
#include "enemy.h"
#include "commonfunction.h"
#include "PowerUp.h"
#include <SDL_mixer.h> // THÊM: Để sử dụng Mix_Chunk

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
        SDL_Texture* explodeTex, Mix_Chunk* explodeSFX); // <--- ĐÃ SỬA

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

    // CÁC BIẾN MỚI CHO HIỆU ỨNG VỤ NỔ <--- THÊM
    SDL_Texture* explodeTexture;
    Mix_Chunk* explodeSound;
    bool isExploding;
    SDL_Rect playerExplodeRect; // Lưu vị trí nổ
    Uint32 explodeStartTime; // Dùng để lưu thời điểm nổ (chứ không dùng để hẹn giờ)
};

#endif // GAMEMAP_H_INCLUDED
