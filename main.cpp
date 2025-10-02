#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <iostream>
#include "Commonfunction.h"
#include "Player.h"
#include "Enemy.h"
#include "GameMap.h"

int main(int argc, char* argv[]) {
    // 1. Init SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        std::cout << "SDL init failed: " << SDL_GetError() << "\n";
        return -1;
    }
    if (TTF_Init() == -1) {
        std::cout << "TTF init failed: " << TTF_GetError() << "\n";
        return -1;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cout << "Mixer init failed: " << Mix_GetError() << "\n";
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("Ne Xe Game",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // 2. Load assets
    SDL_Texture* roadTex   = LoadTexture("image//road.png", renderer);
    SDL_Texture* playerTex = LoadTexture("image//player_car.png", renderer);
    SDL_Texture* enemyTex  = LoadTexture("image//enemy_car.png", renderer);

    if (!roadTex || !playerTex || !enemyTex) {
        std::cout << "Failed to load textures!\n";
        return -1;
    }

    // 3. Tạo game + player
    Game game(renderer, roadTex, enemyTex);
    Player* player = new Player(playerTex, SCREEN_WIDTH/2 - 20, SCREEN_HEIGHT - 100,PLAYER_WIDTH, PLAYER_HEIGHT);
    game.SetPlayer(player);

    // 4. Vòng lặp game
    bool running = true;
    SDL_Event e;
    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = false;
            player->HandleInput(e);
        }

        game.Update();
        game.Render();
        SDL_Delay(16);
    }

    // 5. Cleanup
    delete player;
    SDL_DestroyTexture(roadTex);
    SDL_DestroyTexture(playerTex);
    SDL_DestroyTexture(enemyTex);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_CloseAudio();
    TTF_Quit();
    SDL_Quit();
    return 0;
}

