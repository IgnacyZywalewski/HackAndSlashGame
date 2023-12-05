#include <SDL_image.h>
#include "weapons.h"

Weapon::Weapon(SDL_Renderer* renderer, float x, float y, float w, float h)
    : renderer(renderer), weaponDirection(WeaponDirection::LEFT) {
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;

    SDL_Surface* tmpSurface = IMG_Load("assets/weapon_whip.png");
    weaponTexture = SDL_CreateTextureFromSurface(renderer, tmpSurface);
    SDL_FreeSurface(tmpSurface);
}

Weapon::~Weapon() {
    SDL_DestroyTexture(weaponTexture);
}

void Weapon::setWeaponDirection(WeaponDirection direction) {
    weaponDirection = direction;
}

void Weapon::drawWeapon(SDL_Renderer* renderer) {
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    if (weaponDirection == WeaponDirection::RIGHT) {
        flip = SDL_FLIP_HORIZONTAL;
    }

    SDL_Rect weaponRect = { static_cast<int>(rect.x), static_cast<int>(rect.y),
                            static_cast<int>(rect.w), static_cast<int>(rect.h) };
    SDL_RenderCopyEx(renderer, weaponTexture, nullptr, &weaponRect, 0, nullptr, flip);
}

void Weapon::updatePosition(float playerX, float playerY, float playerW, float playerH) {
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
