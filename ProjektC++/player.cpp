#include <iostream>
#include "player.h"
#include "game.h"

Player::Player(SDL_Renderer* renderer, float x, float y, float w, float h)
    : renderer(renderer), playerTexture(nullptr), facingDirection(Direction::LEFT), health(100) {
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
}

Player::~Player() {
    SDL_DestroyTexture(playerTexture);
}

void Player::spawnPlayer() {
    SDL_Rect playerRect = { static_cast<int>(rect.x), static_cast<int>(rect.y),
                            static_cast<int>(rect.w), static_cast<int>(rect.h) };
    SDL_RenderCopy(renderer, playerTexture, nullptr, &playerRect);
}

void Player::updatePlayerPosition(int screenWidth, int screenHeight, float playerSpeed) {
    const Uint8* keystates = SDL_GetKeyboardState(nullptr);

    double newX = rect.x;
    double newY = rect.y;

    if (keystates[SDL_SCANCODE_UP]) {
        newY -= playerSpeed;  // Przesuniêcie w górê
    }
    if (keystates[SDL_SCANCODE_DOWN]) {
        newY += playerSpeed;  // Przesuniêcie w dó³
    }
    if (keystates[SDL_SCANCODE_LEFT]) {
        newX -= playerSpeed;  // Przesuniêcie w lewo
        facingDirection = Direction::LEFT;
    }
    if (keystates[SDL_SCANCODE_RIGHT]) {
        newX += playerSpeed;  // Przesuniêcie w prawo
        facingDirection = Direction::RIGHT; 
    }

    rect.x = std::max(0.0, std::min(newX, static_cast<double>(screenWidth) - rect.w));
    rect.y = std::max(0.0, std::min(newY, static_cast<double>(screenHeight) - rect.h));
}