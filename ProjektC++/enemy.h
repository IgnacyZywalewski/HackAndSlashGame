#pragma once

#include <SDL.h>

struct RectEnemy {
    float x;
    float y;
    float w;
    float h;
};


class Enemy {
public:
    Enemy(SDL_Renderer* renderer, float x, float y, float w, float h);
    ~Enemy();

    void updateEnemyPosition(float playerX, float playerY, float enemySpeed);
    void draw(SDL_Texture* enemyTexture);
    float getDamage() const { return damage; }
    float getHealth() const { return health; }
    void reduceHealth(float damage) { health -= damage; }

    bool isStopped = false;

    RectEnemy rect;

private:
    float damage = 0.017f; // 1 deamage co 1 sekunde
    float health = 10.0f;
    SDL_Renderer* renderer;
    SDL_Texture* enemyTexture;
};