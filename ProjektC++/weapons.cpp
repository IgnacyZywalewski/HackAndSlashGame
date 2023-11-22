#include <SDL_image.h>

#include "weapons.h"

Weapon::Weapon(SDL_Renderer* renderer, float x, float y, float w, float h)
    :weaponTexture(nullptr), weaponDirection(WeaponDirection::RIGHT) {
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h; 

    SDL_Surface* tmpSurface3 = IMG_Load("assets/weapon_whip.png");
    weaponTexture = SDL_CreateTextureFromSurface(renderer, tmpSurface3);
    SDL_FreeSurface(tmpSurface3);
}

Weapon::~Weapon() {
    SDL_DestroyTexture(weaponTexture);
}

void Weapon::drawWeapon(SDL_Renderer* renderer) {
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    if (weaponDirection == WeaponDirection::LEFT) {
        flip = SDL_FLIP_HORIZONTAL; 
    }
    SDL_Rect weaponRect = { static_cast<int>(rect.x), static_cast<int>(rect.y),
                            static_cast<int>(rect.w), static_cast<int>(rect.h) };
    SDL_RenderCopyEx(renderer, weaponTexture, nullptr, &weaponRect, 0, nullptr, flip); 
}

void Weapon::updatePosition(float playerX, float playerY) {
    rect.x = playerX;
    rect.y = playerY - rect.h / 2;
}

void Weapon::setWeaponDirection(WeaponDirection direction) {
    weaponDirection = direction;
}
