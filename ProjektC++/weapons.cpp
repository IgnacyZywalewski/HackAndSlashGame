#include <SDL_image.h>

#include "weapons.h"

Weapon::Weapon(SDL_Renderer* renderer, float x, float y, float w, float h)
    : weaponDirection(WeaponDirection::RIGHT) {
    rectWeapon.x = x;
    rectWeapon.y = y;
    rectWeapon.w = w;
    rectWeapon.h = h;

    SDL_Surface* tmpSurface = IMG_Load("assets/weapon_whip.png");
    weaponTexture = SDL_CreateTextureFromSurface(renderer, tmpSurface);
    SDL_FreeSurface(tmpSurface);
}

Weapon::~Weapon() {
    SDL_DestroyTexture(weaponTexture);
}

void Weapon::drawWeapon(SDL_Renderer* renderer) {
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    if (weaponDirection == WeaponDirection::LEFT) {
        flip = SDL_FLIP_HORIZONTAL; 
    }
    SDL_Rect weaponRect = { static_cast<int>(rectWeapon.x), static_cast<int>(rectWeapon.y),
                            static_cast<int>(rectWeapon.w), static_cast<int>(rectWeapon.h) };
    SDL_RenderCopyEx(renderer, weaponTexture, nullptr, &weaponRect, 0, nullptr, flip);

}

void Weapon::updatePosition(float playerX, float playerY) {
    rectWeapon.x = playerX;
    rectWeapon.y = playerY - rectWeapon.h / 2;
}

void Weapon::setWeaponDirection(WeaponDirection direction) {
    weaponDirection = direction;
}
