#include "GameMap.h"
#include <cstdlib>
#include "Menu.h"
#include <fstream>
#include <string>

Game::Game(SDL_Renderer* renderer, SDL_Texture* roadTex, const std::vector<SDL_Texture*>& enemyTexList, SDL_Texture* heartTex)
    : renderer(renderer), roadTexture(roadTex), enemyTextures(enemyTexList),
      player(nullptr), frameCount(0), backgroundY(0),
      heartTexture(heartTex), invincible(false), invincibleStart(0),
      score(0), lastScoreTime(SDL_GetTicks())
{
    if (g_gameMode == MODE_EASY) hp = 3;
    else if (g_gameMode == MODE_MEDIUM) hp = 2;
    else hp = 1;

    LoadHighscore();
}

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


    // scroll background
    backgroundY += 5;
    if (backgroundY >= SCREEN_HEIGHT) backgroundY = 0;

    if (invincible && SDL_GetTicks() - invincibleStart > 1000) invincible = false;

    for (auto enemy : enemies) {
        if (!invincible && SDLCommonFunction::CheckCollision(player->GetRect(), enemy->GetRect())) {
            hp--;
            invincible = true;
            invincibleStart = SDL_GetTicks();
            if (hp <= 0) {
                SaveHighscore();
                SDL_Delay(1500);
            }

            break;
        }
    }

    // Cộng điểm mỗi 1 giây
    if (SDL_GetTicks() - lastScoreTime >= 1000) {
        score += 5;
        lastScoreTime = SDL_GetTicks();
    }
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

    // vẽ máu
    for (int i = 0; i < hp; i++) {
        SDL_Rect heartRect = {10 + i * 35, 10, 30, 30};
        SDL_RenderCopy(renderer, heartTexture, NULL, &heartRect);
    }

    // vẽ score
    TTF_Font* font = TTF_OpenFont("font//dlxfont_.ttf", 10);
    if (font) {
        SDL_Color white = {255, 255, 255, 255};

        std::string scoreText = "Score: " + std::to_string(score);
        SDL_Surface* s1 = TTF_RenderText_Solid(font, scoreText.c_str(), white);
        SDL_Texture* t1 = SDL_CreateTextureFromSurface(renderer, s1);
        SDL_Rect rect1 = {SCREEN_WIDTH - s1->w - 10, 10, s1->w, s1->h};
        SDL_RenderCopy(renderer, t1, NULL, &rect1);
        SDL_FreeSurface(s1);
        SDL_DestroyTexture(t1);

        std::string highText = "Highscore: " + std::to_string(highscore);
        SDL_Surface* s2 = TTF_RenderText_Solid(font, highText.c_str(), white);
        SDL_Texture* t2 = SDL_CreateTextureFromSurface(renderer, s2);
        SDL_Rect rect2 = {(SCREEN_WIDTH - s2->w)/2, 10, s2->w, s2->h};
        SDL_RenderCopy(renderer, t2, NULL, &rect2);
        SDL_FreeSurface(s2);
        SDL_DestroyTexture(t2);

        TTF_CloseFont(font);
    }

    SDL_RenderPresent(renderer);
}

void Game::LoadHighscore() {
    std::ifstream file("highscore.txt");
    int easy = 0, medium = 0, hard = 0;
    if (file) {
        file >> easy >> medium >> hard;
    }
    file.close();

    if (g_gameMode == MODE_EASY) highscore = easy;
    else if (g_gameMode == MODE_MEDIUM) highscore = medium;
    else highscore = hard;
}

void Game::SaveHighscore() {
    int easy = 0, medium = 0, hard = 0;
    std::ifstream fin("highscore.txt");
    if (fin) fin >> easy >> medium >> hard;
    fin.close();

    if (g_gameMode == MODE_EASY && score > easy) easy = score;
    else if (g_gameMode == MODE_MEDIUM && score > medium) medium = score;
    else if (g_gameMode == MODE_HARD && score > hard) hard = score;

    std::ofstream fout("highscore.txt");
    fout << easy << " " << medium << " " << hard;
    fout.close();
}
