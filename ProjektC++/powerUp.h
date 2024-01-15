#pragma once

#include <iostream>
#include <SDL.h>

class Player;

struct RectPowerUp {
    float x;
    float y;
    float w;
    float h;
};

class PowerUp {
public:
    PowerUp(SDL_Renderer* renderer, float x, float y, float w, float h);
    virtual ~PowerUp();
    virtual void draw() = 0;
    virtual void applyEffect(Player& player) = 0;

    RectPowerUp rect;

protected:
    SDL_Renderer* renderer;
};

class HealthPowerUp : public PowerUp {
public:
    HealthPowerUp(SDL_Renderer* renderer, float x, float y, float w, float h);
    ~HealthPowerUp() override;

    void draw() override;
    void applyEffect(Player& player) override;

private:
    int healthIncrease = 20;
    SDL_Texture* health_powerUp;
};
