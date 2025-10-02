#include "enemy.h"

Enemy::Enemy(SDL_Texture* tex, int x, int y, int w, int h, int s)
    : Car(tex, x, y, w, h), speed(s) {}

void Enemy::Update() {
    MoveDown(speed);
}
