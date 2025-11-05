#include "Player.h"

Player::Player(SDL_Texture* tex, int x, int y, int w, int h)
    : Car(tex, x, y, w, h) {
    lane = 2; // lane giữa
}

void Player::MoveLeft(int laneWidth) {
    if(lane > 0) {
        lane--;
        rect.x = lane * laneWidth + (laneWidth - rect.w)/2;
    }
}

void Player::MoveRight(int laneWidth) {
    if(lane < LANE_COUNT - 1) {
        lane++;
        rect.x = lane * laneWidth + (laneWidth - rect.w)/2;
    }
}

void Player::MoveUp(int screenHeight, int speed) {
    rect.y -= speed;
    if (rect.y < 0) rect.y = 0; // giới hạn trên
}

void Player::MoveDown(int screenHeight, int speed) {
    rect.y += speed;
    if (rect.y + rect.h > screenHeight) rect.y = screenHeight - rect.h; // Giới hạn dưới
}

void Player::HandleInput(const SDL_Event& e) {
    if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
        case SDLK_LEFT:
            MoveLeft(SCREEN_WIDTH / LANE_COUNT);
            break;
        case SDLK_a:
            MoveLeft(SCREEN_WIDTH / LANE_COUNT);
            break;
        case SDLK_RIGHT:
            MoveRight(SCREEN_WIDTH / LANE_COUNT);
            break;
        case SDLK_d:
            MoveRight(SCREEN_WIDTH / LANE_COUNT);
            break;
        }
    }

    if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
        case SDLK_UP:
        case SDLK_w:
            MoveUp(SCREEN_HEIGHT, 15);
            break;
        case SDLK_DOWN:
        case SDLK_s:
            MoveDown(SCREEN_HEIGHT, 15);
            break;
        }
    }
}

void Player::Update() {
    // giữ player trong màn hình
    if (rect.x < 0) rect.x = 0;
    if (rect.x + rect.w > SCREEN_WIDTH) rect.x = SCREEN_WIDTH - rect.w;
}
