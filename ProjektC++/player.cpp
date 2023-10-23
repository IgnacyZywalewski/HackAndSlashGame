#include "player.h"
#include <iostream>

Player::Player(SDL_Renderer* renderer, int x, int y, int w, int h)
    : renderer(renderer) {
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
}

Player::~Player() {
    
}

void Player::render() {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    int screenWidth, screenHeight;
    SDL_GetRendererOutputSize(renderer, &screenWidth, &screenHeight);

    rect.x = (screenWidth - rect.w) / 2;
    rect.y = (screenHeight - rect.h) / 2;

    SDL_RenderFillRect(renderer, &rect);
}