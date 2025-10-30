#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include "CommonFunction.h"

enum MenuOption {
    MENU_NONE,
    MENU_PLAY,
    MENU_MODE,
    MENU_SCORE,
    MENU_SETTING,
    MENU_QUIT
};

MenuOption ShowMenu(SDL_Renderer* renderer);


enum GameMode {
    MODE_EASY,
    MODE_MEDIUM,
    MODE_HARD
};

extern GameMode g_gameMode;

enum PauseMenu {
    RESULT_CONTINUE_GAME,
    RESULT_RESTART_GAME,
    RESULT_QUIT_TO_MENU,
    RESULT_EXIT_GAME
};

PauseMenu ShowPauseMenu(SDL_Renderer* renderer);

#endif // MENU_H_INCLUDED
