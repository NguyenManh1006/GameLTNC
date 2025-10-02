#include "enemy.h"

Enemy::Enemy(SDL_Texture* tex, int x, int y, int w, int h, int speed)
    : Car(tex, x, y, w, h), speed(speed) {}

void Enemy::Update() {
    MoveDown(speed);
}
