#pragma once

#include <iostream>
#include <SDL.h>
#include <vector>

#include "player.h"
#include "enemy.h"
#include "weapons.h"
#include "render.h"
#include "powerUp.h"

enum class GameState { PLAY, EXIT, END, START_SCREEN };

class Game {
public:
    Game();
    ~Game();

    void run();

private:
    void init(const char* title, int x, int y, int w, int h, Uint32 flags);
    void loadTextures();
    void gameLoop();
    void handleEvents();
    void updateGameEntities(Player& player, std::vector<std::unique_ptr<Enemy>>& enemies, Weapon& weapon);
    void renderGame(Player& player, std::vector<std::unique_ptr<Enemy>>& enemies, std::vector<std::unique_ptr<PowerUp>>& powerUps, Weapon& weapon, Render& render);
    GameState handleCollisions(std::vector<std::unique_ptr<Enemy>>& enemies, RectPlayer& player, Player& playerObject, Weapon& weapon);

    void handlePowerUps(Player& player, std::vector<std::unique_ptr<Enemy>>& enemies, std::vector<std::unique_ptr<PowerUp>>& powerUps);

    void handleStartScreenEvents();
    void handleCharacterSelectionEvents(bool& characterSelected);
    void handleEndGameEvents();

    SDL_Window* window;
    SDL_Renderer* renderer;
    GameState gameState;

    int screenHeight;
    int screenWidth;

    std::string selectedCharacter;

    bool isGamePaused = false;
    bool characterSelected = false;
    bool startGame = false;
    bool quitGame = false;

    float playerSpeed = 2;

    int batsDefeated = 0;
    int skeletonsDefeated = 0;
    int enemiesDefeated = 0;

    int timeSinceLastHealthPowerUp = 0;
    int timeSinceLastFreezePowerUp = 0;
    int timeSinceLastEliminationPowerUp = 0;
    
    int timeFrozenStartTime = 0;
};