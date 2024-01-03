#include <SDL.h>
#include <SDL_image.h>
#include "enemy.h"

Enemy::Enemy(SDL_Renderer* renderer, float x, float y, float w, float h){
    this->renderer = renderer;

    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
}

Enemy::~Enemy() {}

Bat::Bat(SDL_Renderer* renderer, float x, float y, float w, float h)
    : Enemy(renderer, x, y, w, h) {
    enemySpeed = 1.2f;
    damage = 0.017f;
    health = 20.0f;
    
    SDL_Surface* tmpBatSurface = IMG_Load("assets/enemy_bat.png");
    batTexture = SDL_CreateTextureFromSurface(renderer, tmpBatSurface);
    SDL_FreeSurface(tmpBatSurface);
}

Bat::~Bat() {
    SDL_DestroyTexture(batTexture);
}

Skeleton::Skeleton(SDL_Renderer* renderer, float x, float y, float w, float h)
    : Enemy(renderer, x, y, w, h) {
    enemySpeed = 0.8f;
    damage = 0.1f;
    health = 100.0f;
    
    SDL_Surface* tmpSkeletonSurface = IMG_Load("assets/enemy_skeleton.png");
    skeletonTexture = SDL_CreateTextureFromSurface(renderer, tmpSkeletonSurface);
    SDL_FreeSurface(tmpSkeletonSurface);
}

Skeleton::~Skeleton() {
    SDL_DestroyTexture(skeletonTexture);
}

void Bat::draw(float playerX) {
    SDL_Rect enemyRect = { static_cast<int>(rect.x), static_cast<int>(rect.y),
                            static_cast<int>(rect.w), static_cast<int>(rect.h) };

    SDL_RendererFlip flip = SDL_FLIP_NONE;
    if (playerX < rect.x) {
        flip = SDL_FLIP_HORIZONTAL;
    }

    SDL_RenderCopyEx(renderer, batTexture, nullptr, &enemyRect, 0, nullptr, flip);
}

void Skeleton::draw(float playerX) {
    SDL_Rect enemyRect = { static_cast<int>(rect.x), static_cast<int>(rect.y),
                            static_cast<int>(rect.w), static_cast<int>(rect.h) };

    SDL_RendererFlip flip = SDL_FLIP_NONE;
    if (playerX < rect.x) {
        flip = SDL_FLIP_HORIZONTAL;
    }

    SDL_RenderCopyEx(renderer, skeletonTexture, nullptr, &enemyRect, 0, nullptr, flip);
}

void Enemy::updateEnemyPosition(float playerX, float playerY) {
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
