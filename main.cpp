#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <iostream>
#include "CommonFunction.h"
#include "Player.h"
#include "Enemy.h"
#include "GameMap.h"
#include "Menu.h"

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) return -1;
    if (TTF_Init() == -1) { SDL_Quit(); return -1; }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) { TTF_Quit(); SDL_Quit(); return -1; }

    SDL_Window* window = SDL_CreateWindow("Game",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    bool running = true;
    while (running) {
        MenuOption choice = ShowMenu(renderer);
        if (choice == MENU_QUIT) break;

        else if (choice == MENU_PLAY) {
            // --- Load texture ---
            SDL_Texture* roadTex   = LoadTexture("image//road.png", renderer);
            SDL_Texture* playerTex = LoadTexture("image//player_car.png", renderer);
            SDL_Texture* enemyTex1 = LoadTexture("image//enemy_car1.png", renderer);
            SDL_Texture* enemyTex2 = LoadTexture("image//enemy_car2.png", renderer);
            SDL_Texture* enemyTex3 = LoadTexture("image//enemy_car3.png", renderer);
            SDL_Texture* heartTex  = LoadTexture("image//heart.png", renderer);

            if (!roadTex || !playerTex || !enemyTex1 || !enemyTex2 || !enemyTex3 || !heartTex)
                break;

            std::vector<SDL_Texture*> enemyTextures = { enemyTex1, enemyTex2, enemyTex3 };
            Game game(renderer, roadTex, enemyTextures, heartTex);

            Player* player = new Player(playerTex, SCREEN_WIDTH / 2 - 20,
                                        SCREEN_HEIGHT - 100, PLAYER_WIDTH, PLAYER_HEIGHT);
            game.SetPlayer(player);

            bool inGame = true;
            SDL_Event e;

            while (inGame && running) {
                Uint32 frameStart = SDL_GetTicks();
                while (SDL_PollEvent(&e)) {
                    if (e.type == SDL_QUIT) { inGame = false; running = false; }
                    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
                        inGame = false;
                    else
                        player->HandleInput(e);
                }

                game.Update();
                game.Render();

                // Nếu game over
            if (game.GetState() == GameState::GAME_OVER) {
    // Khi GameOver, chờ người chơi chọn MENU hoặc RETRY
    bool waiting = true;
    while (waiting && running) {
        SDL_Event ev;
        while (SDL_PollEvent(&ev)) {
            if (ev.type == SDL_QUIT) { waiting = false; inGame = false; running = false; }
            else {
                game.HandleEvent(ev);  // xử lý click Menu/Retry trong GameMap
                if (game.GetState() == GameState::PLAYING) { // người chơi bấm Retry
                    waiting = false;
                } else if (!game.IsRunning()) { // người chơi bấm Menu
                    waiting = false;
                    inGame = false;
                }
            }
        }
        game.Render();
        SDL_Delay(16);
    }
}

                Uint32 frameTime = SDL_GetTicks() - frameStart;
                if (frameTime < 16) SDL_Delay(16 - frameTime);
            }

            // --- Cleanup ---
            delete player;
            SDL_DestroyTexture(roadTex);
            SDL_DestroyTexture(playerTex);
            SDL_DestroyTexture(enemyTex1);
            SDL_DestroyTexture(enemyTex2);
            SDL_DestroyTexture(enemyTex3);
            SDL_DestroyTexture(heartTex);
        }

        else if (choice == MENU_MODE) {
            bool selecting = true;
            SDL_Event e;
            TTF_Font* font = TTF_OpenFont("font//dlxfont_.ttf", 32);
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

            int hovered = -1;
            while (selecting) {
                while (SDL_PollEvent(&e)) {
                    if (e.type == SDL_QUIT) { selecting = false; running = false; }
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
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderClear(renderer);
                for (int i = 0; i < 3; i++)
                    SDL_RenderCopy(renderer, (i == hovered ? hover[i] : tex[i]), NULL, &rect[i]);
                SDL_RenderPresent(renderer);
            }
            for (int i = 0; i < 3; i++) {
                SDL_DestroyTexture(tex[i]);
                SDL_DestroyTexture(hover[i]);
            }
            TTF_CloseFont(font);
        }

    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_CloseAudio();
    TTF_Quit();
    SDL_Quit();
    return 0;
}
