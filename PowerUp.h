#ifndef POWERUP_H_INCLUDED
#define POWERUP_H_INCLUDED

#include "Car.h"
#include "CommonFunction.h"

class PowerUp : public Car {
public:
    PowerUp(SDL_Texture* tex, int x, int y, int w, int h, int speed, PowerUpType type);
    void Update() override;
    PowerUpType GetType() const;

private:
    int speed;
    PowerUpType type;
};

#endif // POWERUP_H_INCLUDED
