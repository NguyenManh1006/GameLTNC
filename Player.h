#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include "car.h"

class Player : public Car {
public:
    Player(SDL_Texture* tex, int x, int y, int w, int h);
    void MoveLeft(int laneWidth);
    void MoveRight(int laneWidth);
    int GetLane() const;

private:
    int lane;
};

#endif // PLAYER_H_INCLUDED
