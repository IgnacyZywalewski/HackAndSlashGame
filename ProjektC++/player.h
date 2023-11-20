#pragma once

#include <SDL.h>

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
    void clearPlayer(SDL_Renderer* renderer, int screenWidth, int screenHeight);

    RectPlayer rect;
    Direction facingDirection;

private:
    SDL_Renderer* renderer;
    SDL_Texture* playerTexture;
};
