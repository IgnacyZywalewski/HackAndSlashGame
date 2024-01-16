#pragma once

#include <iostream>
#include <SDL.h>
#include <vector>
#include <memory>

class Player;
class Enemy;

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
    virtual void applyEffect(Player& player, std::vector<std::unique_ptr<Enemy>>& enemies) = 0;

    RectPowerUp rect;

protected:
    SDL_Renderer* renderer;
};

class HealthPowerUp : public PowerUp {
public:
    HealthPowerUp(SDL_Renderer* renderer, float x, float y, float w, float h);
    ~HealthPowerUp() override;

    void draw() override;
    void applyEffect(Player& player, std::vector<std::unique_ptr<Enemy>>& enemies) override;

private:
    int healthIncrease = 20;
    SDL_Texture* health_powerUp;
};


class TimeFreezePowerUp : public PowerUp {
public:
    TimeFreezePowerUp(SDL_Renderer* renderer, float x, float y, float w, float h);
    ~TimeFreezePowerUp() override;

    void draw() override;
    void applyEffect(Player& player, std::vector<std::unique_ptr<Enemy>>& enemies) override;

private:
    int freezeTime = 10000;
    SDL_Texture* timeFreeze_powerUp;
};

class EliminationPowerUp : public PowerUp {
public:
    EliminationPowerUp(SDL_Renderer* renderer, float x, float y, float w, float h);
    ~EliminationPowerUp() override;

    void draw() override;
    void applyEffect(Player& player, std::vector<std::unique_ptr<Enemy>>& enemies) override;

private:
    SDL_Texture* elimination_powerUp;
};