#include "Menu.h"
#include <sstream>
#include <iomanip>
#include <algorithm>

GameMode g_gameMode = MODE_EASY;

// Khai báo biến toàn cục cho Setting và Âm nhạc
int g_volume = 50;
Mix_Music* g_music = nullptr;

// Hàm mới: Kiểm soát việc phát/dừng nhạc
void ToggleMusic(bool play) {
    if (play) {
        if (g_music == nullptr) {
            // Tải nhạc nếu chưa tải (Đảm bảo đường dẫn "music//game_music.mp3" là đúng)
            g_music = Mix_LoadMUS("music//game_music.mp3");
            if (g_music == nullptr) {
                // Xử lý lỗi tải nhạc ở đây
                return;
            }
        }

        // Phát nhạc nếu chưa phát. -1 là lặp vô tận.
        if (!Mix_PlayingMusic()) {
            Mix_PlayMusic(g_music, -1);
            UpdateMusicVolume();
        }
    } else {
        // Dừng nhạc nếu đang phát
        if (Mix_PlayingMusic()) {
            Mix_HaltMusic();
        }
    }
}

// SỬA ĐỔI: Cập nhật âm lượng cho Music và TẤT CẢ SFX (bao gồm explode.wav)
void UpdateMusicVolume() {
    // Chuyển g_volume (0-100) sang dải 0-128 của SDL Mixer
    int scaledVolume = (int)((float)g_volume / 100.0f * 128.0f);

    if (g_music != nullptr) {
        Mix_VolumeMusic(scaledVolume); // Cập nhật âm lượng nhạc nền
    }

    // Cập nhật âm lượng SFX chung (-1 áp dụng cho tất cả kênh)
    Mix_Volume(-1, scaledVolume);
}


std::vector<int> LoadHighScores() {
    std::vector<int> scores = {0, 0, 0};
    std::ifstream file("highscore.txt");

    if (file.is_open()) {
        int score_easy, score_medium, score_hard;
        // Giả định thứ tự là Easy, Medium, Hard
        if (file >> score_easy && file >> score_medium && file >> score_hard) {
            scores[0] = score_easy;
            scores[1] = score_medium;
            scores[2] = score_hard;
        }
        file.close();
    }
    return scores;
}

void ShowHighScores(SDL_Renderer* renderer) {
    // 1. Tải Background
    SDL_Texture* backgroundTex = LoadTexture("image//pause.png", renderer);

    // 2. Setup Font và Màu
    TTF_Font* fontTitle = TTF_OpenFont("font//dlxfont_.ttf", 24);
    TTF_Font* fontScore = TTF_OpenFont("font//dlxfont_.ttf", 24);
    TTF_Font* fontButton = TTF_OpenFont("font//dlxfont_.ttf", 24);

    if (!fontTitle || !fontScore || !fontButton) {
        if (backgroundTex) SDL_DestroyTexture(backgroundTex);
        return;
    }

    SDL_Color red = {255, 0, 0, 255};
    SDL_Color white = {255, 255, 255, 255};
    SDL_Color darkWhite = {200, 200, 200, 255};
    SDL_Color black = {0, 0, 0, 255};

    // 3. Load Scores
    std::vector<int> scores = LoadHighScores();
    std::string modes[3] = {"EASY", "MEDIUM", "HARD"};

    // 4. Tạo Title Texture
    SDL_Surface* titleSurface = TTF_RenderText_Solid(fontTitle, "HIGH SCORES", red);
    SDL_Texture* titleTex = SDL_CreateTextureFromSurface(renderer, titleSurface);
    SDL_Rect titleRect = { (SCREEN_WIDTH - titleSurface->w) / 2, 200, titleSurface->w, titleSurface->h };
    SDL_FreeSurface(titleSurface);

    // 5. Tạo Score Textures
    SDL_Texture* scoreTex[3];
    SDL_Rect scoreRect[3];
    int startY = titleRect.y + titleRect.h + 30;
    int lineSpacing = 50;

    for (int i = 0; i < 3; i++) {
        std::string scoreStr = modes[i] + ": " + std::to_string(scores[i]);
        SDL_Surface* s = TTF_RenderText_Solid(fontScore, scoreStr.c_str(), white);
        scoreTex[i] = SDL_CreateTextureFromSurface(renderer, s);
        scoreRect[i].w = s->w;
        scoreRect[i].h = s->h;
        scoreRect[i].x = (SCREEN_WIDTH - s->w) / 2;
        scoreRect[i].y = startY + i * lineSpacing;
        SDL_FreeSurface(s);
    }

    // 6. Tạo nút BACK
    std::string backLabel = "BACK";
    SDL_Surface* backS = TTF_RenderText_Solid(fontButton, backLabel.c_str(), white);
    SDL_Surface* backHoverS = TTF_RenderText_Solid(fontButton, backLabel.c_str(), darkWhite);
    SDL_Texture* backTex = SDL_CreateTextureFromSurface(renderer, backS);
    SDL_Texture* backHoverTex = SDL_CreateTextureFromSurface(renderer, backHoverS);
    SDL_Rect backRect;
    backRect.w = backS->w;
    backRect.h = backS->h;
    backRect.x = 20;
    backRect.y = SCREEN_HEIGHT - backS->h - 20;
    SDL_FreeSurface(backS);
    SDL_FreeSurface(backHoverS);

    // 7. Vòng lặp chính
    bool running = true;
    int hovered = 0; // 0: none, 1: back
    SDL_Event e;
    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = false;
            }
            else if (e.type == SDL_MOUSEMOTION) {
                int mx = e.motion.x, my = e.motion.y;
                hovered = 0;
                if (mx >= backRect.x && mx <= backRect.x + backRect.w &&
                    my >= backRect.y && my <= backRect.y + backRect.h) {
                    hovered = 1;
                }
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN) {
                if (hovered == 1) { // Nếu bấm nút BACK
                    running = false;
                }
            }
        }

        // Vẽ nền
        if (backgroundTex) {
            SDL_RenderCopy(renderer, backgroundTex, NULL, NULL);
        } else {
             SDL_SetRenderDrawColor(renderer, black.r, black.g, black.b, black.a);
             SDL_RenderClear(renderer);
        }

        // Vẽ nội dung
        SDL_RenderCopy(renderer, titleTex, NULL, &titleRect);

        for (int i = 0; i < 3; i++) {
            SDL_RenderCopy(renderer, scoreTex[i], NULL, &scoreRect[i]);
        }

        SDL_RenderCopy(renderer, (hovered == 1 ? backHoverTex : backTex), NULL, &backRect);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    // 8. Dọn dẹp
    SDL_DestroyTexture(titleTex);
    for (int i = 0; i < 3; i++) {
        SDL_DestroyTexture(scoreTex[i]);
    }
    SDL_DestroyTexture(backTex);
    SDL_DestroyTexture(backHoverTex);
    if (backgroundTex) SDL_DestroyTexture(backgroundTex);
    TTF_CloseFont(fontTitle);
    TTF_CloseFont(fontScore);
    TTF_CloseFont(fontButton);
}

void ShowSettingMenu(SDL_Renderer* renderer) {
    // 1. Tải Background
    SDL_Texture* backgroundTex = LoadTexture("image//pause.png", renderer);

    // 2. Setup Font và Màu
    TTF_Font* fontTitle = TTF_OpenFont("font//dlxfont_.ttf", 30);
    TTF_Font* fontLabel = TTF_OpenFont("font//dlxfont_.ttf", 25);
    TTF_Font* fontButton = TTF_OpenFont("font//dlxfont_.ttf", 25);

    if (!fontTitle || !fontLabel || !fontButton) {
        if (backgroundTex) SDL_DestroyTexture(backgroundTex);
        return;
    }

    SDL_Color red = {255, 0, 0, 255};
    SDL_Color white = {255, 255, 255, 255};
    SDL_Color darkWhite = {200, 200, 200, 255};
    SDL_Color black = {0, 0, 0, 255};

    // 3. Tạo Title Texture
    SDL_Surface* titleSurface = TTF_RenderText_Solid(fontTitle, "SETTINGS", red);
    SDL_Texture* titleTex = SDL_CreateTextureFromSurface(renderer, titleSurface);
    SDL_Rect titleRect = { (SCREEN_WIDTH - titleSurface->w) / 2, 200, titleSurface->w, titleSurface->h };
    SDL_FreeSurface(titleSurface);

    // 4. Tạo các nút điều chỉnh âm lượng và nút BACK
    // Khung nút + và - (40x40)
    SDL_Rect minusRect = { SCREEN_WIDTH / 2 - 80, 300, 40, 40 };
    SDL_Rect plusRect = { SCREEN_WIDTH / 2 + 40, 300, 40, 40 };
    SDL_Rect valueRect = { SCREEN_WIDTH / 2 - 30, 300, 60, 40 }; // Hiển thị giá trị

    // Nút BACK
    std::string backLabel = "BACK";
    SDL_Surface* backS = TTF_RenderText_Solid(fontButton, backLabel.c_str(), white);
    SDL_Surface* backHoverS = TTF_RenderText_Solid(fontButton, backLabel.c_str(), darkWhite);
    SDL_Texture* backTex = SDL_CreateTextureFromSurface(renderer, backS);
    SDL_Texture* backHoverTex = SDL_CreateTextureFromSurface(renderer, backHoverS);
    SDL_Rect backRect;
    backRect.w = backS->w;
    backRect.h = backS->h;
    backRect.x = 20;
    backRect.y = SCREEN_HEIGHT - backS->h - 20;
    SDL_FreeSurface(backS);
    SDL_FreeSurface(backHoverS);

    SDL_Texture *minusTex = SDL_CreateTextureFromSurface(renderer, TTF_RenderText_Solid(fontLabel, "-", white));
    SDL_Texture *plusTex = SDL_CreateTextureFromSurface(renderer, TTF_RenderText_Solid(fontLabel, "+", white));

    // 5. Vòng lặp chính
    bool running = true;
    int hovered = 0; // 1: minus, 2: plus, 3: back
    SDL_Event e;

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = false;
            }
            else if (e.type == SDL_MOUSEMOTION) {
                int mx = e.motion.x, my = e.motion.y;
                hovered = 0;
                if (mx >= minusRect.x && mx <= minusRect.x + minusRect.w && my >= minusRect.y && my <= minusRect.y + minusRect.h) {
                    hovered = 1;
                } else if (mx >= plusRect.x && mx <= plusRect.x + plusRect.w && my >= plusRect.y && my <= plusRect.y + plusRect.h) {
                    hovered = 2;
                } else if (mx >= backRect.x && mx <= backRect.x + backRect.w && my >= backRect.y && my <= backRect.y + backRect.h) {
                    hovered = 3;
                }
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN) {
                if (hovered == 1) { //nút giảm
                    g_volume = std::max(0, g_volume - 5);
                    UpdateMusicVolume();
                } else if (hovered == 2) { // nút tăng
                    g_volume = std::min(100, g_volume + 5);
                    UpdateMusicVolume();
                } else if (hovered == 3) { // nút back
                    running = false;
                }
            }
        }

        // 6. Vẽ
        // Vẽ Background
        if (backgroundTex) {
            SDL_RenderCopy(renderer, backgroundTex, NULL, NULL);
        } else {
             SDL_SetRenderDrawColor(renderer, black.r, black.g, black.b, black.a);
             SDL_RenderClear(renderer);
        }

        SDL_RenderCopy(renderer, titleTex, NULL, &titleRect);

        // Vẽ chữ Volume
        SDL_Surface* volumeLabelS = TTF_RenderText_Solid(fontLabel, "VOLUME", white);
        SDL_Texture* volumeLabelTex = SDL_CreateTextureFromSurface(renderer, volumeLabelS);
        SDL_Rect volumeLabelRect = { (SCREEN_WIDTH - volumeLabelS->w) / 2, 250, volumeLabelS->w, volumeLabelS->h };
        SDL_RenderCopy(renderer, volumeLabelTex, NULL, &volumeLabelRect);
        SDL_FreeSurface(volumeLabelS);
        SDL_DestroyTexture(volumeLabelTex);

        // Hiển thị giá trị âm lượng (tính theo %)
        float percent = (float)g_volume / 100.0f * 100.0f;
        std::stringstream ss;
        ss << std::fixed << std::setprecision(0) << percent << "%";

        SDL_Surface* volumeValueS = TTF_RenderText_Solid(fontLabel, ss.str().c_str(), white);
        SDL_Texture* volumeValueTex = SDL_CreateTextureFromSurface(renderer, volumeValueS);
        SDL_RenderCopy(renderer, volumeValueTex, NULL, &valueRect);
        SDL_FreeSurface(volumeValueS);
        SDL_DestroyTexture(volumeValueTex);

        // --- Nút GIẢM (-) ---
        SDL_SetRenderDrawColor(renderer, (hovered == 1 ? darkWhite.r : white.r), (hovered == 1 ? darkWhite.g : white.g), (hovered == 1 ? darkWhite.b : white.b), 255);
        SDL_RenderDrawRect(renderer, &minusRect); // Vẽ khung

        // Lấy kích thước thật của ký tự '-' để căn giữa
        int minusW, minusH;
        SDL_QueryTexture(minusTex, NULL, NULL, &minusW, &minusH);
        SDL_Rect minusTextRect = {
            minusRect.x + (minusRect.w - minusW) / 2, // Căn giữa theo X
            minusRect.y + (minusRect.h - minusH) / 2, // Căn giữa theo Y
            minusW,
            minusH
        };
        SDL_RenderCopy(renderer, minusTex, NULL, &minusTextRect); // Vẽ ký tự đã căn giữa

        // --- Nút TĂNG (+) ---
        SDL_SetRenderDrawColor(renderer, (hovered == 2 ? darkWhite.r : white.r), (hovered == 2 ? darkWhite.g : white.g), (hovered == 2 ? darkWhite.b : white.b), 255);
        SDL_RenderDrawRect(renderer, &plusRect); // Vẽ khung

        // Lấy kích thước thật của ký tự '+' để căn giữa
        int plusW, plusH;
        SDL_QueryTexture(plusTex, NULL, NULL, &plusW, &plusH);
        SDL_Rect plusTextRect = {
            plusRect.x + (plusRect.w - plusW) / 2, // Căn giữa theo X
            plusRect.y + (plusRect.h - plusH) / 2, // Căn giữa theo Y
            plusW,
            plusH
        };
        SDL_RenderCopy(renderer, plusTex, NULL, &plusTextRect); // Vẽ ký tự đã căn giữa

        // Vẽ nút BACK
        SDL_RenderCopy(renderer, (hovered == 3 ? backHoverTex : backTex), NULL, &backRect);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    // 7. Dọn dẹp
    if (backgroundTex) SDL_DestroyTexture(backgroundTex);
    SDL_DestroyTexture(titleTex);
    SDL_DestroyTexture(minusTex);
    SDL_DestroyTexture(plusTex);
    SDL_DestroyTexture(backTex);
    SDL_DestroyTexture(backHoverTex);
    TTF_CloseFont(fontTitle);
    TTF_CloseFont(fontLabel);
    TTF_CloseFont(fontButton);
}

void ShowModeMenu(SDL_Renderer* renderer) {
    // 1. Tải Background
    SDL_Texture* backgroundTex = LoadTexture("image//pause.png", renderer);
    SDL_Color black = {0, 0, 0, 255};

    // 2. Setup Menu Options
    bool selecting = true;
    SDL_Event e;
    TTF_Font* font = TTF_OpenFont("font//dlxfont_.ttf", 32);

    if (!font) {
        if (backgroundTex) SDL_DestroyTexture(backgroundTex);
        return;
    }

    SDL_Color red = {255, 0, 0, 255};
    SDL_Color yellow = {255, 255, 0, 255};
    std::string opts[3] = {"EASY", "MEDIUM", "HARD"};
    SDL_Texture* tex[3];
    SDL_Texture* hover[3];
    SDL_Rect rect[3];
    int startY = (SCREEN_HEIGHT - 3 * 60) / 2;
    for (int i = 0; i < 3; i++) {
        SDL_Surface* s1 = TTF_RenderText_Solid(font, opts[i].c_str(), red);
        SDL_Surface* s2 = TTF_RenderText_Solid(font, opts[i].c_str(), yellow);
        tex[i] = SDL_CreateTextureFromSurface(renderer, s1);
        hover[i] = SDL_CreateTextureFromSurface(renderer, s2);
        rect[i].w = s1->w;
        rect[i].h = s1->h;
        rect[i].x = (SCREEN_WIDTH - s1->w) / 2;
        rect[i].y = startY + i * 80;
        SDL_FreeSurface(s1);
        SDL_FreeSurface(s2);
    }

    // 3. Vòng lặp chính
    int hovered = -1;
    while (selecting) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) { selecting = false; }
            else if (e.type == SDL_MOUSEMOTION) {
                int mx = e.motion.x, my = e.motion.y;
                hovered = -1;
                for (int i = 0; i < 3; i++)
                    if (mx >= rect[i].x && mx <= rect[i].x + rect[i].w &&
                        my >= rect[i].y && my <= rect[i].y + rect[i].h)
                        hovered = i;
            } else if (e.type == SDL_MOUSEBUTTONDOWN && hovered != -1) {
                g_gameMode = (GameMode)hovered;
                selecting = false;
            } else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
                selecting = false;
        }

        // Vẽ nền
        if (backgroundTex) {
            SDL_RenderCopy(renderer, backgroundTex, NULL, NULL);
        } else {
             SDL_SetRenderDrawColor(renderer, black.r, black.g, black.b, black.a);
             SDL_RenderClear(renderer);
        }

        // Vẽ các tùy chọn
        for (int i = 0; i < 3; i++)
            SDL_RenderCopy(renderer, (i == hovered ? hover[i] : tex[i]), NULL, &rect[i]);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    // 4. Dọn dẹp
    if (backgroundTex) SDL_DestroyTexture(backgroundTex);
    for (int i = 0; i < 3; i++) {
        SDL_DestroyTexture(tex[i]);
        SDL_DestroyTexture(hover[i]);
    }
    TTF_CloseFont(font);
}


MenuOption ShowMenu(SDL_Renderer* renderer) {
    SDL_Texture* menuTex = LoadTexture("image//menu.png", renderer);
    if (!menuTex) return MENU_QUIT;

    TTF_Font* font = TTF_OpenFont("font//dlxfont_.ttf", 36);
    if (!font) {
        SDL_DestroyTexture(menuTex);
        return MENU_QUIT;
    }

    SDL_Color red = {255, 0, 0, 255};
    SDL_Color darkRed = {200, 0, 0, 255};

    std::string labels[4] = {"PLAY", "MODE", "SCORE", "SETTING"};
    SDL_Rect buttons[4];
    SDL_Texture* normalTex[4];
    SDL_Texture* hoverTex[4];

    int buttonHeight = 60;
    int spacing = 25;
    int startY = (SCREEN_HEIGHT - (4 * buttonHeight + 3 * spacing)) / 2;

    // Tạo texture chữ
    for (int i = 0; i < 4; i++) {
        SDL_Surface* s1 = TTF_RenderText_Solid(font, labels[i].c_str(), red);
        SDL_Surface* s2 = TTF_RenderText_Solid(font, labels[i].c_str(), darkRed);
        normalTex[i] = SDL_CreateTextureFromSurface(renderer, s1);
        hoverTex[i] = SDL_CreateTextureFromSurface(renderer, s2);
        buttons[i].w = s1->w;
        buttons[i].h = s1->h;
        buttons[i].x = (SCREEN_WIDTH - s1->w) / 2;
        buttons[i].y = startY + i * (buttonHeight + spacing);
        SDL_FreeSurface(s1);
        SDL_FreeSurface(s2);
    }

    bool running = true;
    int hovered = -1;
    MenuOption result = MENU_NONE;
    SDL_Event e;

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                result = MENU_QUIT;
                running = false;
            } else if (e.type == SDL_MOUSEMOTION) {
                int mx = e.motion.x;
                int my = e.motion.y;
                hovered = -1;
                for (int i = 0; i < 4; i++) {
                    if (mx >= buttons[i].x && mx <= buttons[i].x + buttons[i].w &&
                        my >= buttons[i].y && my <= buttons[i].y + buttons[i].h) {
                        hovered = i;
                        break;
                    }
                }
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                if (hovered != -1) {
                    switch (hovered) {
                        case 0: result = MENU_PLAY; running = false; break;
                        case 1: result = MENU_MODE; running = false; break; // Trả về MENU_MODE để main.cpp gọi ShowModeMenu
                        case 2: // Nút SCORE
                            ShowHighScores(renderer);
                            break;
                        case 3: // Nút SETTING
                            ShowSettingMenu(renderer);
                            break;
                    }
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, menuTex, NULL, NULL);

        for (int i = 0; i < 4; i++) {
            SDL_RenderCopy(renderer, (i == hovered ? hoverTex[i] : normalTex[i]), NULL, &buttons[i]);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_DestroyTexture(menuTex);
    for (int i = 0; i < 4; i++) {
        SDL_DestroyTexture(normalTex[i]);
        SDL_DestroyTexture(hoverTex[i]);
    }
    TTF_CloseFont(font);

    return result;
}

PauseMenu ShowPauseMenu(SDL_Renderer* renderer) {

    SDL_Texture* backgroundTex = LoadTexture("image//pause.png", renderer);

    TTF_Font* fontPause = TTF_OpenFont("font//dlxfont_.ttf", 40);
    TTF_Font* fontButton = TTF_OpenFont("font//dlxfont_.ttf", 20);

    SDL_Color red = {255, 0, 0, 255};
    SDL_Color yellow = {255, 255, 0, 255};

    SDL_Surface* titleSurface = TTF_RenderText_Solid(fontPause, "PAUSED", red);
    SDL_Texture* titleTex = SDL_CreateTextureFromSurface(renderer, titleSurface);
    SDL_Rect titleRect = { (SCREEN_WIDTH - titleSurface->w) / 2 + 10, 180, titleSurface->w, titleSurface->h };
    SDL_FreeSurface(titleSurface);

    std::string opts[4] = {"CONTINUE", "RESTART", "SETTING", "QUIT"};
    SDL_Texture* tex[4];
    SDL_Texture* hover[4];
    SDL_Rect rect[4];
    int startY = SCREEN_HEIGHT / 2 - 45;

    for (int i = 0; i < 4; i++) {
        SDL_Surface* s1 = TTF_RenderText_Solid(fontButton, opts[i].c_str(), red);
        SDL_Surface* s2 = TTF_RenderText_Solid(fontButton, opts[i].c_str(), yellow);
        tex[i] = SDL_CreateTextureFromSurface(renderer, s1);
        hover[i] = SDL_CreateTextureFromSurface(renderer, s2);
        rect[i].w = s1->w;
        rect[i].h = s1->h;
        rect[i].x = (SCREEN_WIDTH - s1->w) / 2;
        rect[i].y = startY + i * 50;
        SDL_FreeSurface(s1);
        SDL_FreeSurface(s2);
    }

    bool waiting = true;
    int hovered = -1;
    SDL_Event e;
    PauseMenu result = RESULT_CONTINUE_GAME;

    while (waiting) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                waiting = false;
                result = RESULT_EXIT_GAME;
            }
            else if (e.type == SDL_MOUSEMOTION) {
                int mx = e.motion.x, my = e.motion.y;
                hovered = -1;
                for (int i = 0; i < 4; i++)
                    if (mx >= rect[i].x && mx <= rect[i].x + rect[i].w &&
                        my >= rect[i].y && my <= rect[i].y + rect[i].h)
                        hovered = i;
            } else if (e.type == SDL_MOUSEBUTTONDOWN && hovered != -1) {
                switch (hovered) {
                    case 0: result = RESULT_CONTINUE_GAME; waiting = false; break;
                    case 1: result = RESULT_RESTART_GAME; waiting = false; break;
                    case 2: // Nút SETTING
                        ShowSettingMenu(renderer);
                        break;
                    case 3: result = RESULT_QUIT_TO_MENU; waiting = false; break;
                }
            }
        }

        // vẽ background
        if (backgroundTex) {
            SDL_RenderCopy(renderer, backgroundTex, NULL, NULL);
        } else {
             SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
             SDL_RenderClear(renderer);
        }

        // vẽ chữ paused
        SDL_RenderCopy(renderer, titleTex, NULL, &titleRect);

        // vẽ nút
        for (int i = 0; i < 4; i++)
            SDL_RenderCopy(renderer, (i == hovered ? hover[i] : tex[i]), NULL, &rect[i]);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    // clear
    if (backgroundTex) SDL_DestroyTexture(backgroundTex);
    SDL_DestroyTexture(titleTex);
    if (fontPause) TTF_CloseFont(fontPause);
    if (fontButton) TTF_CloseFont(fontButton);
    for (int i = 0; i < 4; i++) {
        SDL_DestroyTexture(tex[i]);
        SDL_DestroyTexture(hover[i]);
    }

    return result;
}
