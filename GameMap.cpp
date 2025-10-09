#include "GameMap.h"
#include <cstdlib>

Game::Game(SDL_Renderer* renderer, SDL_Texture* roadTex, const std::vector<SDL_Texture*>& enemyTexList)
    : renderer(renderer), roadTexture(roadTex), enemyTextures(enemyTexList),
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
    // spawn enemy mỗi 60 frame
    if (frameCount % 60 == 0) {
        int laneWidth = SCREEN_WIDTH / LANE_COUNT;
        int lane = rand() % LANE_COUNT;

        // chọn random texture enemy
        int texIndex = rand() % enemyTextures.size();
        SDL_Texture* tex = enemyTextures[texIndex];

        // random speed mỗi enemy
        int speed = 4 + rand() % 5; // từ 3 -> 7

        Enemy* enemy = new Enemy(tex,
                                 lane * laneWidth + (laneWidth - ENEMY_WIDTH) / 2,
                                 -ENEMY_HEIGHT,
                                 ENEMY_WIDTH, ENEMY_HEIGHT,
                                 speed);

        enemies.push_back(enemy);
    }

    // update enemy
    for (auto enemy : enemies) {
        enemy->Update();
    }

     // xóa enemy ra khỏi màn hình
    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(),
            [](Enemy* e) {
                if (e->GetRect().y > SCREEN_HEIGHT) {
                    delete e;
                    return true;
                }
                return false;
            }),
        enemies.end()
    );

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
