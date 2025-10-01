#ifndef CAR_H_INCLUDED
#define CAR_H_INCLUDED

#include "CommonFunction.h"
class Car {
public:
    Car(SDL_Texture* tex, int x, int y, int w, int h);
    virtual ~Car() {}

    virtual void Update() {}
    void Render(SDL_Renderer* renderer);
    void MoveDown(int speed);
    SDL_Rect GetRect() const;
    void SetRect(int x, int y);

protected:
    SDL_Rect rect;
    SDL_Texture* texture;
};

#endif // CAR_H_INCLUDED
