#ifndef COMMONFUNCTION_H_INCLUDED
#define COMMONFUNCTION_H_INCLUDED

#define POWERUP_WIDTH 40
#define POWERUP_HEIGHT 40
#define MAX_HP 3
#define SHIELD_DURATION 10000
#define POWERUP_SPAWN 10000

#include <windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <iostream>
#include <vector>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <fstream>
#include <algorithm>
#include <ctime>

static SDL_Window* g_window = NULL;
static SDL_Renderer* g_screen = NULL;
static SDL_Event g_event;

const int SCREEN_WIDTH = 400;
const int SCREEN_HEIGHT = 600;
const int ROAD_WIDTH = 200;
const int LANE_COUNT = 5;
const int PLAYER_WIDTH = 60;
const int PLAYER_HEIGHT = 90;
const int ENEMY_WIDTH = 60;
const int ENEMY_HEIGHT = 90;
const int SPEED = 5;

const int COLOR_KEY_R = 167;
const int COLOR_KEY_G = 175;
const int COLOR_KEY_B = 180;

const int RENDER_DRAW_COLOR = 0Xff;

enum class PowerUpType {
    SHIELD,
    HEAL
};


typedef struct Input {
    int left_ , right_;

};

namespace SDLCommonFunction {
    inline bool CheckCollision(const SDL_Rect& a, const SDL_Rect& b) {
        return SDL_HasIntersection(&a, &b);
    }
}

inline SDL_Texture* LoadTexture(const std::string& path, SDL_Renderer* renderer) {
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface) {
        std::cout << "Failed to load image: " << IMG_GetError() << "\n";
        return nullptr;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}


#endif // COMMONFUNCTION_H_INCLUDED
