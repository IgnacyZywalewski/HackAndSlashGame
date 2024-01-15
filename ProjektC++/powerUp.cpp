#include "powerUp.h"
#include "player.h"

#include <SDL_image.h>
#include <SDL.h>

PowerUp::PowerUp(SDL_Renderer* renderer, float x, float y, float w, float h) {
    this->renderer = renderer;
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
}

PowerUp::~PowerUp() {}


HealthPowerUp::HealthPowerUp(SDL_Renderer* renderer, float x, float y, float w, float h)
    : PowerUp(renderer, x, y, w, h) {

    SDL_Surface* tmpHealthPowerUpSurface = IMG_Load("assets/health_powerUp.png");
    health_powerUp = SDL_CreateTextureFromSurface(renderer, tmpHealthPowerUpSurface);
    SDL_FreeSurface(tmpHealthPowerUpSurface);
}

HealthPowerUp::~HealthPowerUp() {}

void HealthPowerUp::draw() {
    SDL_Rect powerUpRect = { static_cast<int>(rect.x), static_cast<int>(rect.y),
                             static_cast<int>(rect.w), static_cast<int>(rect.h) };
    SDL_RenderCopy(renderer, health_powerUp, nullptr, &powerUpRect);
}

void HealthPowerUp::applyEffect(Player& player) {
    if (player.getHealth() < 100) {
        player.increaseHealth(20);
        if (player.getHealth() > 100) {
            player.setHealth(100);
        }
    }
}