#include "Menu.h"
#include "sstream"

using namespace std;

GameMode g_gameMode = MODE_EASY;

MenuOption ShowMenu(SDL_Renderer* renderer) {
    SDL_Texture* menuTex = LoadTexture("image//menu.png", renderer);
    if (!menuTex) return MENU_QUIT;

    TTF_Font* font = TTF_OpenFont("font//dlxfont_.ttf", 36);
    if (!font) {
        SDL_DestroyTexture(menuTex);
        return MENU_QUIT;
    }

    // Đổi toàn bộ sang màu đỏ
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
                        case 1: result = MENU_MODE; running = false; break;
                        case 2: // Nút SCORE
                            ShowHighScores(renderer); // Gọi hàm hiển thị highscore
                            break; // Không thoát vòng lặp, tiếp tục hiển thị menu
                        case 3: result = MENU_SETTING; running = false; break;
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
                waiting = false;
                switch (hovered) {
                    case 0: result = RESULT_CONTINUE_GAME; break;
                    case 1: result = RESULT_RESTART_GAME; break;
                    case 2: result = RESULT_CONTINUE_GAME; break;
                    case 3: result = RESULT_QUIT_TO_MENU; break;
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

vector<int> LoadHighScores() {

    vector<int> scores = {0, 0, 0};
    ifstream file("highscore.txt");

    if (file.is_open()) {
        int score_easy, score_medium, score_hard;

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
    // font , màu
    TTF_Font* fontTitle = TTF_OpenFont("font//dlxfont_.ttf", 30);
    // Dùng font nhỏ hơn cho nút BACK
    TTF_Font* fontScore = TTF_OpenFont("font//dlxfont_.ttf", 30);
    TTF_Font* fontButton = TTF_OpenFont("font//dlxfont_.ttf", 25); // Font cho nút BACK

    if (!fontTitle || !fontScore || !fontButton) return;

    SDL_Color red = {255, 0, 0, 255};
    SDL_Color white = {255, 255, 255, 255};
    SDL_Color darkWhite = {200, 200, 200, 255};
    SDL_Color black = {0, 0, 0, 255};

    //  Load Scores
    std::vector<int> scores = LoadHighScores();
    std::string modes[3] = {"EASY", "MEDIUM", "HARD"};

    // highscore texture
    SDL_Surface* titleSurface = TTF_RenderText_Solid(fontTitle, "HIGHSCORES", red);
    SDL_Texture* titleTex = SDL_CreateTextureFromSurface(renderer, titleSurface);
    SDL_Rect titleRect = { (SCREEN_WIDTH - titleSurface->w) / 2, 100, titleSurface->w, titleSurface->h };
    SDL_FreeSurface(titleSurface);

    // texture score
    SDL_Texture* scoreTex[3];
    SDL_Rect scoreRect[3];
    int startY = titleRect.y + titleRect.h + 50;
    int lineSpacing = 60;

    for (int i = 0; i < 3; i++) {
        std::string scoreStr = modes[i] + ": " + std::to_string(scores[i]);
        SDL_Surface* s = TTF_RenderText_Solid(fontScore, scoreStr.c_str(), white);
        scoreTex[i] = SDL_CreateTextureFromSurface(renderer, s);
        scoreRect[i].w = s->w;
        scoreRect[i].h = s->h;
        scoreRect[i].x = (SCREEN_WIDTH - s->w) / 2; // Canh giữa
        scoreRect[i].y = startY + i * lineSpacing;
        SDL_FreeSurface(s);
    }

    // back
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

    bool running = true;
    int hovered = 0;
    SDL_Event e;
    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = false;
            }
            else if (e.type == SDL_MOUSEMOTION) {
                int mx = e.motion.x, my = e.motion.y;
                hovered = 0;
                // Kiểm tra hover nút BACK
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

        // Vẽ
        SDL_SetRenderDrawColor(renderer, black.r, black.g, black.b, black.a);
        SDL_RenderClear(renderer);

        // Vẽ HIGH SCORES
        SDL_RenderCopy(renderer, titleTex, NULL, &titleRect);

        // Vẽ điểm số
        for (int i = 0; i < 3; i++) {
            SDL_RenderCopy(renderer, scoreTex[i], NULL, &scoreRect[i]);
        }

        // Vẽ nút BACK
        SDL_RenderCopy(renderer, (hovered == 1 ? backHoverTex : backTex), NULL, &backRect);


        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    // clerar
    SDL_DestroyTexture(titleTex);
    for (int i = 0; i < 3; i++) {
        SDL_DestroyTexture(scoreTex[i]);
    }
    SDL_DestroyTexture(backTex);
    SDL_DestroyTexture(backHoverTex);
    TTF_CloseFont(fontTitle);
    TTF_CloseFont(fontScore);
    TTF_CloseFont(fontButton);

}
