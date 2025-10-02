#include "GameMap.h"
#include <cstdlib>

Game::Game(SDL_Renderer* renderer, SDL_Texture* roadTex, SDL_Texture* enemyTex)
    : renderer(renderer), roadTexture(roadTex), enemyTexture(enemyTex),
      player(nullptr), frameCount(0), backgroundY(0) {}

Game::~Game() {
    for (auto e : enemies) {
        delete e;
    }
    enemies.clear();
}

void Game::SetPlayer(Player* p) {
    player = p;
}

void Game::Update() {
    frameCount++;

    // spawn enemy mỗi 60 frame
    if (frameCount % 60 == 0) {
        int laneWidth = SCREEN_WIDTH / 3;
        int lane = rand() % 3;
        Enemy* enemy = new Enemy(enemyTexture,
                                 lane * laneWidth + (laneWidth - 40) / 2,
                                 -60, 40, 60,
                                 5);
        enemies.push_back(enemy);
    }

    // update enemy
    for (auto enemy : enemies) {
        enemy->Update();
    }

    // check collision
    for (auto enemy : enemies) {
        if (SDLCommonFunction::CheckCollision(player->GetRect(), enemy->GetRect())) {
            SDL_Log("Game Over!");
        }
    }

    // scroll background
    backgroundY += 5;
    if (backgroundY >= SCREEN_HEIGHT) backgroundY = 0;
}

void Game::Render() {
    SDL_RenderClear(renderer);

    // vẽ nền cuộn
    SDL_Rect dst1 = {0, backgroundY, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_Rect dst2 = {0, backgroundY - SCREEN_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderCopy(renderer, roadTexture, NULL, &dst1);
    SDL_RenderCopy(renderer, roadTexture, NULL, &dst2);

    // vẽ player
    if (player) player->Render(renderer);

    // vẽ enemy
    for (auto enemy : enemies) {
        enemy->Render(renderer);
    }

    SDL_RenderPresent(renderer);
}
