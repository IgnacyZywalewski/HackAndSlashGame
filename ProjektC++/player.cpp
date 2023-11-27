#include <iostream>
#include <SDL_image.h>

#include "player.h"
#include "game.h"
#include "weapons.h"

Player::Player(SDL_Renderer* renderer, float x, float y, float w, float h)
    : renderer(renderer), facingDirection(Direction::RIGHT), weapon(renderer, x + w, y + h / 2, 80, 30) {
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;

    SDL_Surface* tmpSurface = IMG_Load("assets/player.png");
    playerTexture = SDL_CreateTextureFromSurface(renderer, tmpSurface);
    SDL_FreeSurface(tmpSurface);
}

Player::~Player() {
    SDL_DestroyTexture(playerTexture);
}

void Player::spawnPlayer() {
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    if (facingDirection == Direction::LEFT) {
        flip = SDL_FLIP_HORIZONTAL;
    }
    SDL_Rect playerRect = { static_cast<int>(rect.x), static_cast<int>(rect.y),
                            static_cast<int>(rect.w), static_cast<int>(rect.h) };
    SDL_RenderCopyEx(renderer, playerTexture, nullptr, &playerRect, 0, nullptr, flip);

    weapon.drawWeapon(renderer); 
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


    if (facingDirection == Direction::LEFT) {
        weapon.setWeaponDirection(WeaponDirection::LEFT);
        weapon.updatePosition(rect.x - rect.w - 10, rect.y + rect.h / 2);
    }
    else {
        weapon.setWeaponDirection(WeaponDirection::RIGHT);
        weapon.updatePosition(rect.x + rect.w - 10, rect.y + rect.h / 2);
    }
    
}