#pragma once

#include <SDL.h>
#include "weapons.h"

enum class Direction { LEFT, RIGHT };

struct RectPlayer {
    float x;
    float y;
    float w;
    float h;
};


class Player {
public:
    Player(SDL_Renderer* renderer, float x, float y, float w, float h);
    ~Player();

    void spawnPlayer();
    void updatePlayerPosition(int screenWidth, int screenHeight, float playerSpeed);
    float getHealth() const { return health; }
    void reduceHealth(float damage) { health -= damage; }
    void increaseHealth(float value) { health += value; }
    void setHealth(float value) { health = value; }

    SDL_Texture* playerTexture; 
    RectPlayer rect;
    Direction facingDirection;
    bool flag = false;
    

private:
    float health = 100.0f;
    SDL_Renderer* renderer;
};
