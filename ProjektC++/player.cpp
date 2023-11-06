#include <iostream>
#include "player.h"

Player::Player(SDL_Renderer* renderer, float x, float y, float w, float h)
    : renderer(renderer) {
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
}

Player::~Player() {}

void Player::spawnPlayer() {
    int centerX = static_cast<int>(rect.x + rect.w / 2);
    int centerY = static_cast<int>(rect.y + rect.h / 2);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    SDL_Rect playerRect;
    playerRect.w = static_cast<int>(rect.w);
    playerRect.h = static_cast<int>(rect.h);
    playerRect.x = centerX - playerRect.w / 2;
    playerRect.y = centerY - playerRect.h / 2;

    SDL_RenderFillRect(renderer, &playerRect);
}

void Player::updatePlayerPosition(int screenWidth, int screenHeight) {
    const Uint8* keystates = SDL_GetKeyboardState(nullptr);

    double newX = rect.x;
    double newY = rect.y;

    if (keystates[SDL_SCANCODE_UP]) {
        newY -= 3.0;  // Przesuniêcie w górê
    }
    if (keystates[SDL_SCANCODE_DOWN]) {
        newY += 3.0;  // Przesuniêcie w dó³
    }
    if (keystates[SDL_SCANCODE_LEFT]) {
        newX -= 3.0;  // Przesuniêcie w lewo
    }
    if (keystates[SDL_SCANCODE_RIGHT]) {
        newX += 3.0;  // Przesuniêcie w prawo
    }

    // Tu stosujemy now¹ strukturê Rect
    rect.x = std::max(0.0, std::min(newX, static_cast<double>(screenWidth) - rect.w));
    rect.y = std::max(0.0, std::min(newY, static_cast<double>(screenHeight) - rect.h));
}

void Player::clearPlayer(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    SDL_Rect playerRect;
    playerRect.w = static_cast<int>(rect.w);
    playerRect.h = static_cast<int>(rect.h);
    playerRect.x = static_cast<int>(rect.x);
    playerRect.y = static_cast<int>(rect.y);

    SDL_RenderFillRect(renderer, &playerRect);
}