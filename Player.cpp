#include "Player.h"

Player::Player(SDL_Texture* tex, int x, int y, int w, int h)
    : Car(tex, x, y, w, h) {
    lane = 1; // lane giữa
}

void Player::MoveLeft(int laneWidth) {
    if(lane > 0) {
        lane--;
        rect.x = lane * laneWidth + (laneWidth - rect.w)/2;
    }
}

void Player::MoveRight(int laneWidth) {
    if(lane < 2) {
        lane++;
        rect.x = lane * laneWidth + (laneWidth - rect.w)/2;
    }
}

void Player::HandleInput(const SDL_Event& e) {
    if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
        case SDLK_LEFT:
            rect.x -= 20;
            break;
        case SDLK_RIGHT:
            rect.x += 20;
            break;
        }
    }
}

void Player::Update() {
    // giữ player trong màn hình
    if (rect.x < 0) rect.x = 0;
    if (rect.x + rect.w > SCREEN_WIDTH) rect.x = SCREEN_WIDTH - rect.w;
}
