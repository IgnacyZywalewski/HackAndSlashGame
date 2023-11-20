#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

#include "enemy.h"

Enemy::Enemy(SDL_Renderer* renderer, float x, float y, float w, float h, float moveSpeed) {
    this->renderer = renderer;

    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;

    this->speed = moveSpeed;

    enemyTexture = IMG_LoadTexture(renderer, "assets/enemy_skeleton.png");
    if (!enemyTexture) {
        std::cerr << "Nie mozna zaladowac tekstury wroga: " << IMG_GetError() << "\n";
    }
}

Enemy::~Enemy() {
    SDL_DestroyTexture(enemyTexture);
}

void Enemy::draw() {
    SDL_Rect enemyRect = { static_cast<int>(rect.x), static_cast<int>(rect.y),
                            static_cast<int>(rect.w), static_cast<int>(rect.h) };
    SDL_RenderCopy(renderer, enemyTexture, nullptr, &enemyRect);
}

void Enemy::updateEnemyPosition(float playerX, float playerY) {
    float dx = playerX - rect.x;
    float dy = playerY - rect.y;

    float length = sqrt(dx * dx + dy * dy);

    dx /= length;
    dy /= length;

    rect.x += dx * speed;
    rect.y += dy * speed;
}

void Enemy::clearEnemy(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); // Ustaw kolor na przezroczysty (Alpha = 0)

    SDL_Rect clearRect;
    clearRect.x = static_cast<int>(rect.x);
    clearRect.y = static_cast<int>(rect.y);
    clearRect.w = static_cast<int>(rect.w);
    clearRect.h = static_cast<int>(rect.h);

    SDL_RenderFillRect(renderer, &clearRect); // Wype³nij obszar wroga kolorem przezroczystym
}
