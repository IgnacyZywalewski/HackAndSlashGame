#pragma once

#include <SDL.h>

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
    void updatePlayerPosition(int screenWidth, int screenHeight);
    void clearPlayer(SDL_Renderer* renderer);

    RectPlayer rect;

private:
    SDL_Renderer* renderer;
};
