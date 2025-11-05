#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include "car.h"

class Player : public Car {
public:
    Player(SDL_Texture* tex, int x, int y, int w, int h);
    void MoveLeft(int laneWidth);
    void MoveRight(int laneWidth);
    void MoveUp(int screenHeight, int speed);
    void MoveDown(int screenHeight, int speed);
    int GetLane() const;

    void HandleInput(const SDL_Event& e);
    void Update();

private:
    int lane;
};

#endif // PLAYER_H_INCLUDED
