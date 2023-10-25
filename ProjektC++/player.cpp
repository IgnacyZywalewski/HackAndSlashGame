#include <iostream>

#include "player.h"

Player::Player(SDL_Renderer* renderer, double x, double y, int w, int h)
    : renderer(renderer) {
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
}

Player::~Player() {}

void Player::spawnPlayer() {
    int centerX = rect.x + rect.w / 2;
    int centerY = rect.y + rect.h / 2;

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    SDL_Rect playerRect;
    playerRect.w = 50;  
    playerRect.h = 50;  
    playerRect.x = centerX - playerRect.w / 2;
    playerRect.y = centerY - playerRect.h / 2;

    SDL_RenderFillRect(renderer, &playerRect);
}

void Player::updatePlayerPosition(int screenWidth, int screenHeight) {
    const Uint8* keystates = SDL_GetKeyboardState(nullptr);

    float newX = rect.x;
    float newY = rect.y;

    if (keystates[SDL_SCANCODE_UP]) {
        newY -= 3;  // Przesuniêcie w górê
    }
    if (keystates[SDL_SCANCODE_DOWN]) {
        newY += 3;  // Przesuniêcie w dó³
    }
    if (keystates[SDL_SCANCODE_LEFT]) {
        newX -= 3;  // Przesuniêcie w lewo
    }
    if (keystates[SDL_SCANCODE_RIGHT]) {
        newX += 3;  // Przesuniêcie w prawo
    }

    rect.x = std::max(0.0f, std::min(newX, static_cast<float>(screenWidth - rect.w)));
    rect.y = std::max(0.0f, std::min(newY, static_cast<float>(screenHeight - rect.h)));
}


void Player::clearPlayer(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
}