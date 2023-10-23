#include "player.h"

Player::Player(SDL_Renderer* renderer, double x, double y, int w, int h)
    : renderer(renderer) {
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
}

Player::~Player() {
    
}

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

void Player::updatePlayerPosition() {
    const Uint8* keystates = SDL_GetKeyboardState(nullptr);

    if (keystates[SDL_SCANCODE_UP]) {
        rect.y -= 1.0;  // Przesuniêcie w górê
    }
    if (keystates[SDL_SCANCODE_DOWN]) {
        rect.y += 1.0;  // Przesuniêcie w dó³
    }
    if (keystates[SDL_SCANCODE_LEFT]) {
        rect.x -= 1.0;  // Przesuniêcie w lewo
    }
    if (keystates[SDL_SCANCODE_RIGHT]) {
        rect.x += 1.0;  // Przesuniêcie w prawo
    }
}

void Player::clearPlayer(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // Kolor t³a (np. czarny)
    SDL_RenderFillRect(renderer, &rect);
}

