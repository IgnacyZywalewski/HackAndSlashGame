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
    ~Weapon();

    void drawWhipWeapon(SDL_Renderer* renderer);
    void updateWhipPosition(float playerX, float playerY, float playerW, float playerH);

    void setWeaponDirection(WeaponDirection direction);
    float getDamage() const { return damage; }

    void drawFireballWeapon(std::vector<Weapon>& fireballs, SDL_Renderer* renderer);
    //void fireFireball( float playerX, float playerY, float playerW, float playerH);
    void updateFireballPosition(float fireballSpeed, float playerX, float playerY, float playerW, float playerH);

    //void checkFireballCollision(std::vector<Enemy>& enemies, int enemiesDefeted);

    RectWeapon rect;
    WeaponDirection weaponDirection;
    SDL_Texture* weaponTexture;
    SDL_Renderer* renderer;
    std::vector<Weapon> fireballs;

private:
    float damage = 1.0f;
};
