#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <iostream>
#include "Commonfunction.h"
#include "Player.h"
#include "Enemy.h"
#include "GameMap.h"
#include "Menu.h"

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) return -1;
    if (TTF_Init() == -1) { SDL_Quit(); return -1; }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) { TTF_Quit(); SDL_Quit(); return -1; }

    SDL_Window* window = SDL_CreateWindow("Ne Xe Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    bool running = true;
    while (running) {
        MenuOption choice = ShowMenu(renderer);
        if (choice == MENU_QUIT) {
            running = false;
            break;
        }
        else if (choice == MENU_PLAY) {
            SDL_Texture* roadTex   = LoadTexture("image//road.png", renderer);
            SDL_Texture* playerTex = LoadTexture("image//player_car.png", renderer);
            SDL_Texture* enemyTex1 = LoadTexture("image//enemy_car1.png", renderer);
            SDL_Texture* enemyTex2 = LoadTexture("image//enemy_car2.png", renderer);
            SDL_Texture* enemyTex3 = LoadTexture("image//enemy_car3.png", renderer);
            if (!roadTex || !playerTex || !enemyTex1 || !enemyTex2 || !enemyTex3) break;

            std::vector<SDL_Texture*> enemyTextures = { enemyTex1, enemyTex2, enemyTex3 };
            Game game(renderer, roadTex, enemyTextures);
            Player* player = new Player(playerTex, SCREEN_WIDTH / 2 - 20, SCREEN_HEIGHT - 100, PLAYER_WIDTH, PLAYER_HEIGHT);
            game.SetPlayer(player);

            bool inGame = true;
            SDL_Event e;
            while (inGame) {
                Uint32 frameStart = SDL_GetTicks();
                while (SDL_PollEvent(&e)) {
                    if (e.type == SDL_QUIT) { running = false; inGame = false; }
                    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) inGame = false;
                    player->HandleInput(e);
                }
                game.Update();
                game.Render();
                Uint32 frameTime = SDL_GetTicks() - frameStart;
                if (frameTime < 16) SDL_Delay(16 - frameTime);
            }

            delete player;
            SDL_DestroyTexture(roadTex);
            SDL_DestroyTexture(playerTex);
            SDL_DestroyTexture(enemyTex1);
            SDL_DestroyTexture(enemyTex2);
            SDL_DestroyTexture(enemyTex3);
        }
        else if (choice == MENU_MODE) {
            SDL_SetRenderDrawColor(renderer, 0, 0, 64, 255);
            SDL_RenderClear(renderer);
            SDL_RenderPresent(renderer);
            SDL_Delay(1000);
        }
        else if (choice == MENU_SCORE) {
            SDL_SetRenderDrawColor(renderer, 64, 0, 0, 255);
            SDL_RenderClear(renderer);
            SDL_RenderPresent(renderer);
            SDL_Delay(1000);
        }
        else if (choice == MENU_SETTING) {
            SDL_SetRenderDrawColor(renderer, 0, 64, 0, 255);
            SDL_RenderClear(renderer);
            SDL_RenderPresent(renderer);
            SDL_Delay(1000);
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_CloseAudio();
    TTF_Quit();
    SDL_Quit();
    return 0;
}
