#include "Car.h"

Car::Car(SDL_Texture* tex, int x, int y, int w, int h) {
    texture = tex;
    rect = {x, y, w, h};
}
