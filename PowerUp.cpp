#include "PowerUp.h"

PowerUp::PowerUp(SDL_Texture* tex, int x, int y, int w, int h, int speed, PowerUpType type)
    : Car(tex, x, y, w, h), speed(speed), type(type) {}

void PowerUp::Update() {
    rect.y += speed;
}

PowerUpType PowerUp::GetType() const {
    return type;
}
