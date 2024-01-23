#include <SDL_image.h>
#include <iostream>
#include "weapons.h"

Weapon::Weapon(SDL_Renderer* renderer, float x, float y, float w, float h)
    : renderer(renderer), weaponDirection(WeaponDirection::LEFT), weaponTexture(nullptr) {
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
}

Weapon::~Weapon() {
    SDL_DestroyTexture(weaponTexture);
}

void Weapon::setWeaponDirection(WeaponDirection direction) {
    weaponDirection = direction;
}

Whip::Whip(SDL_Renderer* renderer, float x, float y, float w, float h)
    : Weapon(renderer, x, y, w, h) {
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
}

Whip::~Whip() {
    SDL_DestroyTexture(weaponTexture);
}

void Whip::drawWeapon(SDL_Renderer* renderer) {
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    if (weaponDirection == WeaponDirection::RIGHT) {
        flip = SDL_FLIP_HORIZONTAL;
    }

    SDL_Rect weaponRect = { static_cast<int>(rect.x), static_cast<int>(rect.y),
                            static_cast<int>(rect.w), static_cast<int>(rect.h) };
    SDL_RenderCopyEx(renderer, weaponTexture, nullptr, &weaponRect, 0, nullptr, flip);
}

void Whip::updatePosition(float playerX, float playerY, float playerW, float playerH) {
    const Uint8* keystates = SDL_GetKeyboardState(nullptr);

    if (keystates[SDL_SCANCODE_LEFT]) {
        setWeaponDirection(WeaponDirection::LEFT);
    }
    else if (keystates[SDL_SCANCODE_RIGHT]) {
        setWeaponDirection(WeaponDirection::RIGHT);
    }

    if (weaponDirection == WeaponDirection::LEFT) {
        rect.x = playerX - playerW - 20;
        rect.y = playerY + (playerH / 2) - 10;
    }
    if (weaponDirection == WeaponDirection::RIGHT) {
        rect.x = playerX + playerW - 20;
        rect.y = playerY + (playerH / 2) - 10;
    }
}

Fireball::Fireball(SDL_Renderer* renderer, float x, float y, float w, float h)
    : Weapon(renderer, x, y, w, h) {
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
}

Fireball::~Fireball() {
    SDL_DestroyTexture(weaponTexture);
}

void Fireball::drawWeapon(SDL_Renderer* renderer) {
    for (auto it = fireballs.begin(); it != fireballs.end(); it++) {
        SDL_RendererFlip flip = SDL_FLIP_NONE;
        if (it->weaponDirection == WeaponDirection::RIGHT) {
            flip = SDL_FLIP_HORIZONTAL;
        }
        SDL_Rect weaponRect = { static_cast<int>(it->rect.x), static_cast<int>(it->rect.y),
                                static_cast<int>(it->rect.w), static_cast<int>(it->rect.h) };
        SDL_RenderCopyEx(renderer, weaponTexture, nullptr, &weaponRect, 0, nullptr, flip);
    }
}


void Fireball::createNewFireball(float playerX, float playerY, float playerW, float playerH) {
    int currentTime = SDL_GetTicks();
    static int lastFireballTime = 0;

    const Uint8* keystates = SDL_GetKeyboardState(nullptr);

    if (keystates[SDL_SCANCODE_LEFT]) {
        setWeaponDirection(WeaponDirection::LEFT);
    }
    if (keystates[SDL_SCANCODE_RIGHT]) {
        setWeaponDirection(WeaponDirection::RIGHT);
    }

    if (currentTime - lastFireballTime >= timeBetweenShoot) {
        lastFireballTime = currentTime;

        float fireballX = 0, fireballY = 0;
        if (weaponDirection == WeaponDirection::LEFT) {
            fireballX = playerX - playerW - 20;
            fireballY = playerY + (playerH / 2) - 10;
        }
        else if (weaponDirection == WeaponDirection::RIGHT) {
            fireballX = playerX + playerW - 20;
            fireballY = playerY + (playerH / 2) - 10;
        }

        Fireball fireball(renderer, fireballX, fireballY, 80, 20);
        fireball.setWeaponDirection(weaponDirection);
        fireballs.push_back(fireball);
    }
}

void Fireball::updatePosition(float playerX, float playerY, float playerW, float playerH) {
    createNewFireball(playerX, playerY, playerW, playerH);

    for (auto it = fireballs.begin(); it != fireballs.end();) {
        if (it->weaponDirection == WeaponDirection::LEFT) {
            it->rect.x -= fireballSpeed;
            if (it->rect.x + it->rect.w < 0) {
                it = fireballs.erase(it);
            }
            else {
                ++it;
            }
        }
        else if (it->weaponDirection == WeaponDirection::RIGHT) {
            it->rect.x += fireballSpeed;
            if (it->rect.x > 1360) {
                it = fireballs.erase(it);
            }
            else {
                ++it;
            }
        }
    }
}

