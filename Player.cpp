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
