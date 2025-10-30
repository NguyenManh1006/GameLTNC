#include "GameMap.h"
#include <cstdlib>
#include "Menu.h"
#include <fstream>
#include <string>

Game::Game(SDL_Renderer* renderer, SDL_Texture* roadTex, const std::vector<SDL_Texture*>& enemyTexList, SDL_Texture* heartTex)
    : renderer(renderer),
      roadTexture(roadTex),
      enemyTextures(enemyTexList),
      player(nullptr),
      frameCount(0),
      backgroundY(0),
      heartTexture(heartTex),
      invincible(false),
      invincibleStart(0),
      score(0),
      lastScoreTime(SDL_GetTicks()),
      state(GameState::PLAYING),
      isRunning(true)
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
                state = GameState::GAME_OVER;
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

    // game over
    if (state == GameState::GAME_OVER) {
        RenderGameOver();
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

void Game::RenderGameOver() {
    SDL_Color white = {255, 255, 255, 255};
    TTF_Font* font = TTF_OpenFont("font//dlxfont_.ttf", 20);
    if (!font) return;

    SDL_Texture* bg = IMG_LoadTexture(renderer, "image/gameover.jpg");
        if (bg) {
            int imgW, imgH;
            SDL_QueryTexture(bg, NULL, NULL, &imgW, &imgH);

            // Tính toán vị trí hiển thị giữa màn hình
            int targetW = imgW / 2; // thu nhỏ 50%
            int targetH = imgH / 2;
            SDL_Rect dstRect;
            dstRect.x = SCREEN_WIDTH / 2 - targetW / 2;
            dstRect.y = SCREEN_HEIGHT / 2 - targetH / 2;
            dstRect.w = targetW;
            dstRect.h = targetH;

            SDL_RenderCopy(renderer, bg, NULL, &dstRect);
            SDL_DestroyTexture(bg);
        }

    // Game Over text
    SDL_Surface* sTitle = TTF_RenderText_Solid(font, "GAME OVER", white);
    SDL_Texture* tTitle = SDL_CreateTextureFromSurface(renderer, sTitle);
    SDL_Rect rTitle = {SCREEN_WIDTH/2 - sTitle->w/2, SCREEN_HEIGHT/4 + 80, sTitle->w, sTitle->h};
    SDL_RenderCopy(renderer, tTitle, NULL, &rTitle);
    SDL_FreeSurface(sTitle);
    SDL_DestroyTexture(tTitle);

    // Score
    std::string sText = "Score: " + std::to_string(score);
    SDL_Surface* sScore = TTF_RenderText_Solid(font, sText.c_str(), white);
    SDL_Texture* tScore = SDL_CreateTextureFromSurface(renderer, sScore);
    SDL_Rect rScore = {SCREEN_WIDTH/2 - sScore->w/2, rTitle.y + 60, sScore->w, sScore->h};
    SDL_RenderCopy(renderer, tScore, NULL, &rScore);
    SDL_FreeSurface(sScore);
    SDL_DestroyTexture(tScore);

    // Highscore
    std::string hText = "Highscore: " + std::to_string(highscore);
    SDL_Surface* sHigh = TTF_RenderText_Solid(font, hText.c_str(), white);
    SDL_Texture* tHigh = SDL_CreateTextureFromSurface(renderer, sHigh);
    SDL_Rect rHigh = {SCREEN_WIDTH/2 - sHigh->w/2, rScore.y + 40, sHigh->w, sHigh->h};
    SDL_RenderCopy(renderer, tHigh, NULL, &rHigh);
    SDL_FreeSurface(sHigh);
    SDL_DestroyTexture(tHigh);

    // Nút Menu (trái)
    std::string menuText = "MENU";
    SDL_Surface* sMenu = TTF_RenderText_Solid(font, menuText.c_str(), white);
    SDL_Texture* tMenu = SDL_CreateTextureFromSurface(renderer, sMenu);
    SDL_Rect rMenu = {SCREEN_WIDTH/2 - 130 - sMenu->w/2, rHigh.y + 60, sMenu->w, sMenu->h};
    SDL_RenderCopy(renderer, tMenu, NULL, &rMenu);
    SDL_FreeSurface(sMenu);
    SDL_DestroyTexture(tMenu);

    // Nút Retry (phải)
    std::string retryText = "RETRY";
    SDL_Surface* sRetry = TTF_RenderText_Solid(font, retryText.c_str(), white);
    SDL_Texture* tRetry = SDL_CreateTextureFromSurface(renderer, sRetry);
    SDL_Rect rRetry = {SCREEN_WIDTH/2 + 130 - sRetry->w/2, rHigh.y + 60, sRetry->w, sRetry->h};
    SDL_RenderCopy(renderer, tRetry, NULL, &rRetry);
    SDL_FreeSurface(sRetry);
    SDL_DestroyTexture(tRetry);

    TTF_CloseFont(font);

    SDL_RenderPresent(renderer);
}

void Game::HandleEvent(SDL_Event e) {
    // Thoát game bằng nút X
    if (e.type == SDL_QUIT) {
        isRunning = false;
    }

    // Nếu đang chơi bình thường
    if (state == GameState::PLAYING) {
        if (player) {
            player->HandleInput(e); // Gọi xử lý di chuyển của xe người chơi
        }
    }

    // Nếu ở màn hình Game Over
    else if (state == GameState::GAME_OVER && e.type == SDL_MOUSEBUTTONDOWN) {
        int x = e.button.x;
        int y = e.button.y;

        // Xác định vùng nút Menu và Retry (tọa độ khớp với RenderGameOver)
        SDL_Rect menuRect = {SCREEN_WIDTH/2 - 200, SCREEN_HEIGHT/2 + 60, 120, 50};
        SDL_Rect retryRect = {SCREEN_WIDTH/2 + 80, SCREEN_HEIGHT/2 + 60, 120, 50};

        // Click vào MENU → quay lại menu chính
        if (x >= menuRect.x && x <= menuRect.x + menuRect.w &&
            y >= menuRect.y && y <= menuRect.y + menuRect.h) {
            isRunning = false;
        }

        // Click vào RETRY → chơi lại
        if (x >= retryRect.x && x <= retryRect.x + retryRect.w &&
            y >= retryRect.y && y <= retryRect.y + retryRect.h) {
            ResetGame();                // Bạn cần viết hàm ResetGame() để reset hp, score, enemy,...
            state = GameState::PLAYING; // Chuyển lại trạng thái đang chơi
        }
    }
}

void Game::ResetGame() {
    // Xóa dữ liệu cũ
    enemies.clear();
    frameCount = 0;
    backgroundY = 0;
    invincible = false;
    invincibleStart = 0;
    score = 0;
    lastScoreTime = SDL_GetTicks();

    if (g_gameMode == MODE_EASY) hp = 3;
    else if (g_gameMode == MODE_MEDIUM) hp = 2;
    else hp = 1;

    // Đặt lại vị trí player
    if (player) {
        SDL_Rect rect = player->GetRect();
        player->SetRect(SCREEN_WIDTH / 2 - rect.w / 2,
                        SCREEN_HEIGHT - rect.h - 20);
    }

    state = GameState::PLAYING; // quay lại trạng thái đang chơi
}

