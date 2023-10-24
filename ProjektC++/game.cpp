#include "game.h"
#include "player.h"
#include "enemy.h"

#include <windows.h>


Game::Game()
    : window(nullptr), renderer(nullptr), screenHeight(768), screenWidth(1360), gameState(GameState::PLAY) {
}

Game::~Game() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Game::run() {
    init("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
    gameLoop();
}

void Game::init(const char* title, int x, int y, int w, int h, Uint32 flags) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cerr << "Nie uda³o siê zainicjowaæ SDL: " << SDL_GetError() << "\n";
    }

    window = SDL_CreateWindow(title, x, y, w, h, flags);
    if (!window) {
        std::cerr << "Nie uda³o siê utworzyæ okna: " << SDL_GetError() << "\n";
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        std::cerr << "Nie uda³o siê utworzyæ renderera: " << SDL_GetError() << "\n";
    }
}

void Game::handleEvents() {
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type) {
    case SDL_QUIT:
        gameState = GameState::EXIT;
        break;
    case SDL_KEYDOWN:
        switch (event.key.keysym.sym) {
        case SDLK_ESCAPE:
            gameState = GameState::EXIT;
            break;
        }
        break;
    }
}


// Funkcja do generowania wrogów
void generateEnemies(std::vector<Enemy>& enemies, SDL_Renderer* renderer, int screenWidth, int screenHeight) {
    if (rand() % 100 < 0.5) { // Przyk³ad: Nowy wróg co 1% czasu
        int randomX = rand() % screenWidth;
        int randomY = rand() % screenHeight;
        float moveSpeed = 1.0f; // Przyk³adowa prêdkoœæ ruchu wroga

        enemies.push_back(Enemy(renderer, randomX, randomY, 20, 20, moveSpeed));
    }
}
/*
bool checkPlayerEnemyCollision(const SDL_Rect& playerRect, const SDL_Rect& enemyRect) {
    // SprawdŸ, czy prostok¹t gracza nachodzi na prostok¹t wroga
    if (playerRect.x + playerRect.w <= enemyRect.x || playerRect.x >= enemyRect.x + enemyRect.w ||
        playerRect.y + playerRect.h <= enemyRect.y || playerRect.y >= enemyRect.y + enemyRect.h) {
        // Brak kolizji miêdzy graczem a tym wrogiem
    }
    else {
        // Kolizja miêdzy graczem a tym wrogiem
        return true;
    }
    // Brak kolizji miêdzy graczem a ¿adnym wrogiem
    return false;
}*/


void drawEnemies(std::vector<Enemy>& enemies) {
    for (Enemy& enemy : enemies) {
        enemy.draw();
    }
}

// Funkcja do aktualizowania pozycji wrogów
void updateEnemies(std::vector<Enemy>& enemies, float playerX, float playerY) {
    for (Enemy& enemy : enemies) {
        enemy.updateEnemyPosition(playerX, playerY);
    }
}

/*
// Funkcja do obs³ugi kolizji z graczem
void handleCollisions(std::vector<Enemy>& enemies, SDL_Rect playerRect) {
    for (int i = enemies.size() - 1; i >= 0; i--) {
        if (checkPlayerEnemyCollision(playerRect, enemies[i].rect)) {
            // Obs³uga kolizji, np. koniec gry
            enemies.erase(enemies.begin() + i); // Usuñ wroga po kolizji
        }
    }
}*/


void Game::gameLoop() {
    Player player(renderer, screenWidth / 2 - 25, screenHeight / 2 - 25, 50, 50);
    std::vector<Enemy> enemies;

    while (gameState != GameState::EXIT) {
        handleEvents();

        player.clearPlayer(renderer);
        Sleep(1);
        player.updatePlayerPosition(screenWidth, screenHeight);
        player.spawnPlayer();


        for (Enemy& enemy : enemies) {
            enemy.clearEnemy(renderer);
        }
        Sleep(10);
        generateEnemies(enemies, renderer, screenWidth, screenHeight);
        updateEnemies(enemies, player.rect.x, player.rect.y);
        drawEnemies(enemies);

        //handleCollisions(enemies, player.rect);

        SDL_RenderPresent(renderer);
    }
}
