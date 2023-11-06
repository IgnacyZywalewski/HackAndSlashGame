#pragma once

#include <SDL.h>
#include <vector>

struct RectEnemy {
    float x;
    float y;
    float w;
    float h;
};


class Enemy {
public:
    Enemy(SDL_Renderer* renderer, float x, float y, int w, int h, float moveSpeed);
    ~Enemy();

    void updateEnemyPosition(float playerX, float playerY);
    void draw();
    void clearEnemy(SDL_Renderer* renderer);

    RectEnemy rect;
    
private:
    SDL_Renderer* renderer;
    float speed;
};