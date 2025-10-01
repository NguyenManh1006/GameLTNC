#include "Car.h"

Car::Car(SDL_Texture* tex, int x, int y, int w, int h) {
    texture = tex;
    rect = {x, y, w, h};
}

void Car::Render(SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, texture, NULL, &rect);
}

void Car::MoveDown(int speed) {
    rect.y += speed;
}

void Car::GetRect() const {
    return rect;
}

void Car::SetRect(int x, int y) {
    rect.x = x;
    rect.y = y;
}
