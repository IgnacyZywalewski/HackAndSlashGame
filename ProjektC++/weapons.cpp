#include <SDL_image.h>
#include <iostream>
#include "weapons.h"

Weapon::Weapon(SDL_Renderer* renderer, float x, float y, float w, float h)
    : renderer(renderer), weaponDirection(WeaponDirection::LEFT) {
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

void Weapon::drawWhipWeapon(SDL_Renderer* renderer) {
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    if (weaponDirection == WeaponDirection::RIGHT) {
        flip = SDL_FLIP_HORIZONTAL;
    }

    SDL_Rect weaponRect = { static_cast<int>(rect.x), static_cast<int>(rect.y),
                            static_cast<int>(rect.w), static_cast<int>(rect.h) };
    SDL_RenderCopyEx(renderer, weaponTexture, nullptr, &weaponRect, 0, nullptr, flip);
}

void Weapon::updateWhipPosition(float playerX, float playerY, float playerW, float playerH) {
    const Uint8* keystates = SDL_GetKeyboardState(nullptr);

    if (keystates[SDL_SCANCODE_LEFT]) {
        setWeaponDirection(WeaponDirection::LEFT);
    }
    if (keystates[SDL_SCANCODE_RIGHT]) {
        setWeaponDirection(WeaponDirection::RIGHT);
    }

    if (weaponDirection == WeaponDirection::LEFT) {
        rect.x = playerX - playerW - 20;
        rect.y = playerY + (playerH / 2) - 10;
    }
    else if (weaponDirection == WeaponDirection::RIGHT) {
        rect.x = playerX + playerW - 20;
        rect.y = playerY + (playerH / 2) - 10;
    }
}

void Weapon::drawFireballWeapon(std::vector<Weapon>& fireballs, SDL_Renderer* renderer) {
    for (auto it = fireballs.begin(); it != fireballs.end(); it++) {
        SDL_RendererFlip flip = SDL_FLIP_NONE;
        if (weaponDirection == WeaponDirection::RIGHT) {
            flip = SDL_FLIP_HORIZONTAL;
        }

        SDL_Rect weaponRect = { static_cast<int>(rect.x), static_cast<int>(rect.y),
                                static_cast<int>(rect.w), static_cast<int>(rect.h) };
        SDL_RenderCopyEx(renderer, weaponTexture, nullptr, &weaponRect, 0, nullptr, flip);
    }
}

void Weapon::updateFireballPosition(float fireballSpeed, float playerX, float playerY, float playerW, float playerH) {
    static int lastFireballTime = 0;
    int currentTime = SDL_GetTicks();

    if (currentTime - lastFireballTime >= 1000) {
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

        Weapon fireball(renderer, fireballX, fireballY, 80, 20);
        fireballs.push_back(fireball);
    }

    for (auto& fireball : fireballs) {
        if (fireball.weaponDirection == WeaponDirection::LEFT) {
            rect.x -= fireballSpeed;
        }
        else if (fireball.weaponDirection == WeaponDirection::RIGHT) {
            rect.x += fireballSpeed;
        }
    }
}


/*
void Weapon::fireFireball(float playerX, float playerY, float playerW, float playerH) {
    float fireballX = 0, fireballY = 0;

    if (weaponDirection == WeaponDirection::LEFT) {
        fireballX = playerX - playerW - 20;
        fireballY = playerY + (playerH / 2) - 10;
    }
    else if (weaponDirection == WeaponDirection::RIGHT) {
        fireballX = playerX + playerW - 20;
        fireballY = playerY + (playerH / 2) - 10;
    }

    Weapon fireball(renderer, fireballX, fireballY, 80, 30);
    fireballs.push_back(fireball);
}


void Weapon::checkFireballCollision(std::vector<Enemy>& enemies, int enemiesDefeted) {
    for (auto it = fireballs.begin(); it != fireballs.end();) {
        SDL_Rect fireballRect = { static_cast<int>(it->rect.x), static_cast<int>(it->rect.y), static_cast<int>(it->rect.w), static_cast<int>(it->rect.h) };

        for (auto enemyIt = enemies.begin(); enemyIt != enemies.end();) {
            SDL_Rect enemyRect = { static_cast<int>(enemyIt->rect.x), static_cast<int>(enemyIt->rect.y), static_cast<int>(enemyIt->rect.w), static_cast<int>(enemyIt->rect.h) };

            if (SDL_HasIntersection(&fireballRect, &enemyRect)) {
                enemyIt->reduceHealth(getDamage()); // Przyk³adowe zadawane obra¿enia fireballa
                if (enemyIt->getHealth() <= 0) {
                    enemyIt = enemies.erase(enemyIt);
                    enemiesDefeted++;
                }
                else {
                    ++enemyIt;
                }

                it = fireballs.erase(it);
                break;
            }
            else {
                ++enemyIt;
            }
        }

        if (it != fireballs.end()) {
            ++it;
        }
    }
}*/