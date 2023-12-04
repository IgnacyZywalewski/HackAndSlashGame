#include <SDL.h>
#include <SDL_image.h>
#include "enemy.h"

Enemy::Enemy(SDL_Renderer* renderer, float x, float y, float w, float h)
    : enemyTexture(nullptr) {
    this->renderer = renderer;

    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
}

Enemy::~Enemy() {
    SDL_DestroyTexture(enemyTexture);
}

void Enemy::draw(SDL_Texture* enemyTexture) {
    SDL_Rect enemyRect = { static_cast<int>(rect.x), static_cast<int>(rect.y),
                            static_cast<int>(rect.w), static_cast<int>(rect.h) };
    SDL_RenderCopy(renderer, enemyTexture, nullptr, &enemyRect);
}

void Enemy::updateEnemyPosition(float playerX, float playerY, float enemySpeed) {
    if (!isStopped) {
        float dx = playerX - rect.x;
        float dy = playerY - rect.y;

        float length = sqrt(dx * dx + dy * dy);

        dx /= length;
        dy /= length;

        rect.x += dx * enemySpeed;
        rect.y += dy * enemySpeed;
    }
}
