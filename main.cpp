#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <iostream>
#include <vector>
#include <string>
#include "CommonFunction.h"
#include "Player.h"
#include "Enemy.h"
#include "GameMap.h"
#include "Menu.h"
#include "PowerUp.h"

extern Mix_Music* g_music;

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
        // Nhạc nền phải TẮT khi ở Menu chính
        MenuOption choice = ShowMenu(renderer);
        if (choice == MENU_QUIT) break;

        else if (choice == MENU_PLAY) {
            // load texture
            SDL_Texture* roadTex   = LoadTexture("image//road.png", renderer);
            SDL_Texture* playerTex = LoadTexture("image//player_car.png", renderer);
            SDL_Texture* enemyTex1 = LoadTexture("image//enemy_car1.png", renderer);
            SDL_Texture* enemyTex2 = LoadTexture("image//enemy_car2.png", renderer);
            SDL_Texture* enemyTex3 = LoadTexture("image//enemy_car3.png", renderer);
            SDL_Texture* heartTex  = LoadTexture("image//heart.png", renderer);
            SDL_Texture* shieldTex = LoadTexture("image//shield.png", renderer);
            SDL_Texture* healTex   = LoadTexture("image//heal.png", renderer);
            SDL_Texture* explodeTex = LoadTexture("image//explode.png", renderer);
            Mix_Chunk* explodeSound = Mix_LoadWAV("music//explode.wav");

            if (!roadTex || !playerTex || !enemyTex1 || !enemyTex2 || !enemyTex3 || !heartTex || !shieldTex || !healTex || !explodeTex || !explodeSound)
                break;

            std::vector<SDL_Texture*> enemyTextures = { enemyTex1, enemyTex2, enemyTex3 };

            bool shouldRestart = false;

            do {
                shouldRestart = false;
                PauseMenu loopResult = RESULT_QUIT_TO_MENU;

                Game game(renderer, roadTex, enemyTextures, heartTex, shieldTex, healTex, explodeTex, explodeSound);
                Player* player = new Player(playerTex, SCREEN_WIDTH / 2 - 20,
                                            SCREEN_HEIGHT - 100, PLAYER_WIDTH, PLAYER_HEIGHT);
                game.SetPlayer(player);

                bool inGame = true;
                SDL_Event e;

                // BẬT NHẠC KHI BẮT ĐẦU CHƠI GAME
                ToggleMusic(true);

                // chơi game
                while (inGame && running) {
                    Uint32 frameStart = SDL_GetTicks();

                    // xử lý sự kiện
                    while (SDL_PollEvent(&e)) {
                        if (e.type == SDL_QUIT) { inGame = false; running = false; loopResult = RESULT_EXIT_GAME; }
                        else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
                            // pause game
                            ToggleMusic(false); // tắt nhạc khi pause
                            loopResult = ShowPauseMenu(renderer);

                            if (loopResult == RESULT_CONTINUE_GAME) {
                                ToggleMusic(true); // bật nhạc khi bấm continume
                            } else {
                                inGame = false; // Thoát vòng lặp game
                            }
                        }
                        else
                            player->HandleInput(e);
                    }

                    if (inGame) {
                        game.Update();
                        game.Render();
                    }

                    // gameover
                    if (game.GetState() == GameState::GAME_OVER) {
                        ToggleMusic(false); //tắt nhạc
                        bool waiting = true;
                        while (waiting && running) {
                            SDL_Event ev;
                            while (SDL_PollEvent(&ev)) {
                                if (ev.type == SDL_QUIT) {
                                    waiting = false; inGame = false; running = false; loopResult = RESULT_EXIT_GAME;
                                }
                                else {
                                    game.HandleEvent(ev);
                                    if (game.GetState() == GameState::PLAYING) { // bấm retry
                                        waiting = false;
                                        inGame = false;
                                        loopResult = RESULT_RESTART_GAME;
                                    } else if (!game.IsRunning()) { // bấm menu
                                        waiting = false;
                                        inGame = false;
                                        loopResult = RESULT_QUIT_TO_MENU;
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

                // Đảm bảo nhạc TẮT khi thoát khỏi vòng lặp game
                ToggleMusic(false);

                if (loopResult == RESULT_RESTART_GAME) {
                    shouldRestart = true;
                } else if (loopResult == RESULT_EXIT_GAME) {
                    running = false;
                }

                // Clear
                delete player;

            } while (shouldRestart && running);

            // xóa texture
            SDL_DestroyTexture(roadTex);
            SDL_DestroyTexture(playerTex);
            SDL_DestroyTexture(enemyTex1);
            SDL_DestroyTexture(enemyTex2);
            SDL_DestroyTexture(enemyTex3);
            SDL_DestroyTexture(heartTex);
            SDL_DestroyTexture(shieldTex);
            SDL_DestroyTexture(healTex);
            SDL_DestroyTexture(explodeTex);
            Mix_FreeChunk(explodeSound);
        }

        else if (choice == MENU_MODE) {
            ShowModeMenu(renderer);
        }
    }

    // Dọn dẹp cuối cùng
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    // Giải phóng tài nguyên nhạc
    if (g_music) Mix_FreeMusic(g_music);
    Mix_CloseAudio();

    TTF_Quit();
    SDL_Quit();
    return 0;
}
