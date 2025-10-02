#ifndef ENEMY_H_INCLUDED
#define ENEMY_H_INCLUDED

#include "car.h"

class Enemy : public Car {
public:
    Enemy(SDL_Texture* tex, int x, int y, int w, int h, int speed);
    void Update() override;

private:
    int speed;
};

#endif // ENEMY_H_INCLUDED
