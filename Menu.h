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
#endif // MENU_H_INCLUDED
