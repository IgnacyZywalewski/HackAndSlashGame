#pragma once
#include <SDL.h>
#include <vector>

#include "player.h"
#include "enemy.h"

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
    virtual ~Weapon();

    virtual void drawWeapon(SDL_Renderer* renderer) = 0;
    virtual void updatePosition(float playerX, float playerY, float playerW, float playerH) = 0;

    void setWeaponDirection(WeaponDirection direction);
    float getDamage() const { return damage; }

    RectWeapon rect;
    WeaponDirection weaponDirection;
    SDL_Texture* weaponTexture;
    SDL_Renderer* renderer;

private:
    float damage = 1.0f;
};


class Whip : public Weapon {
public:
    Whip(SDL_Renderer* renderer, float x, float y, float w, float h);
    ~Whip();

    void drawWeapon(SDL_Renderer* renderer) override;
    void updatePosition(float playerX, float playerY, float playerW, float playerH) override;

};

class Fireball : public Weapon {
public:
    Fireball(SDL_Renderer* renderer, float x, float y, float w, float h);
    ~Fireball();

    void drawWeapon(SDL_Renderer* renderer) override;
    void updatePosition(float playerX, float playerY, float playerW, float playerH) override;
    void createNewFireball(float playerX, float playerY, float playerW, float playerH);

    std::vector<Fireball> fireballs;
    float fireballSpeed = 2.0f;
    float timeBetweenShoot = 2000;
};
