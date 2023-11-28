#pragma once

#include <SDL.h>

enum class WeaponDirection { LEFT, RIGHT };

struct RectWeapon {
    float x;
    float y;
    float w;
    float h;
};

class Weapon {
public:
    Weapon(SDL_Renderer* renderer, float x, float y, float w, float h);
    ~Weapon();

    void drawWeapon(SDL_Renderer* renderer);
    void updatePosition(float playerX, float playerY, float playerW, float playerH);
    void setWeaponDirection(WeaponDirection direction);
    float getDamage() const { return damage; }

    RectWeapon rect;
    WeaponDirection weaponDirection; 

private:
    SDL_Texture* weaponTexture;
    SDL_Renderer* renderer;
    float damage = 0.5f;
};
