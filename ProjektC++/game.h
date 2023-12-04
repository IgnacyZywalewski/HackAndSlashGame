#pragma once

#include <iostream>
#include <SDL.h>
#include <vector>
#include "player.h"
#include "enemy.h"
#include "weapons.h"
#include "render.h"

enum class GameState { PLAY, EXIT };

class Game {
public:
    Game();
    ~Game();

    void run();

private:
    void init(const char* title, int x, int y, int w, int h, Uint32 flags);
    void gameLoop();
    void handleEvents();
    void updateGameEntities(Player& player, std::vector<Enemy>& enemies, Weapon& weapon);
    void renderGame(Player& player, std::vector<Enemy>& enemies, Weapon& weapon, Render& render);

    SDL_Window* window;
    SDL_Renderer* renderer;

    const int screenHeight;
    const int screenWidth;
    GameState gameState;
};
