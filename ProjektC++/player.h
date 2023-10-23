#pragma once

#include <SDL.h>

class Player {  
public:
    Player(SDL_Renderer* renderer, double x, double y, int w, int h);
    ~Player();

    void spawnPlayer();
    void updatePlayerPosition();
    void clearPlayer(SDL_Renderer* renderer);

private:
    SDL_Renderer* renderer;
    SDL_Rect rect;
};
