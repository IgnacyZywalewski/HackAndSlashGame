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
    virtual ~Enemy();

    void updateEnemyPosition(float playerX, float playerY);
    virtual void draw(float playerX) = 0;
    float getDamage() const { return damage; }
    float getHealth() const { return health; }
    void reduceHealth(float damage) { health -= damage; }

    bool isStopped = false;
    RectEnemy rect;

protected:
    SDL_Renderer* renderer;
    float enemySpeed = 0;
    float damage = 0;
    float health = 0;
};


class Bat : public Enemy {
public:
    Bat(SDL_Renderer* renderer, float x, float y, float w, float h);
    ~Bat() override;

    void draw(float playerX) override;

private:
    SDL_Texture* batTexture;
};


class Skeleton : public Enemy {
public:
    Skeleton(SDL_Renderer* renderer, float x, float y, float w, float h);
    ~Skeleton() override;

    void draw(float playerX) override;

private:
    SDL_Texture* skeletonTexture;
};
