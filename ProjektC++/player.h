#pragma once

#include <SDL.h>

class Player {
public:
    Player(SDL_Renderer* renderer, int x, int y, int w, int h);
    ~Player();

    void render();

private:
    SDL_Renderer* renderer;
    SDL_Rect rect;
};
