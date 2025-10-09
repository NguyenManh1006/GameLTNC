#include "Menu.h"

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
                        case 0: result = MENU_PLAY; break;
                        case 1: result = MENU_MODE; break;
                        case 2: result = MENU_SCORE; break;
                        case 3: result = MENU_SETTING; break;
                    }
                    running = false;
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
