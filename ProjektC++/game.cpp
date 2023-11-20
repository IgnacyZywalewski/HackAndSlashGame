#include <windows.h>
#include <vector>
#include <SDL_image.h>

#include "game.h"
#include "player.h"
#include "enemy.h"

float playerWidth = 50;
float playerHeight = 50;
float playerSpeed = 3.0;
float enemySpeed = 1.0;

SDL_Texture* playerTexture = nullptr;
//SDL_Texture* enemyTexture = nullptr;
//SDL_Texture* backgroundTexture = nullptr;

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
        std::cerr << "Nie udalo sie utworzyc okna: " << SDL_GetError() << "\n";
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        std::cerr << "Nie udalo sie utworzyc renderera: " << SDL_GetError() << "\n";
    }

    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        std::cerr << "Nie mozna zainicjowaæ SDL_Image: " << IMG_GetError() << "\n";
    }

    /*backgroundTexture = IMG_LoadTexture(renderer, "assets/background.png");
    if (!backgroundTexture) {
        std::cerr << "Nie mozna zaladowac tekstury tla: " << IMG_GetError() << "\n";
    }*/

    playerTexture = IMG_LoadTexture(renderer, "assets/player.png");
    if (!playerTexture) {
        std::cerr << "Nie mozna zaladowac tekstury gracza: " << IMG_GetError() << "\n";
    }
}



void generateEnemies(std::vector<Enemy>& enemies, SDL_Renderer* renderer, int screenWidth, int screenHeight) {
    if (rand() % 100 < 1) { // Nowy wróg co 1% czasu
        int randomX = rand() % screenWidth;
        int randomY = rand() % screenHeight;

        enemies.push_back(Enemy(renderer, randomX, randomY, 20, 20, enemySpeed));
    }
}

void drawEnemies(std::vector<Enemy>& enemies) {
    for (Enemy& enemy : enemies) {
        enemy.draw();
    }
}

void updateEnemies(std::vector<Enemy>& enemies, float playerX, float playerY) {
    for (Enemy& enemy : enemies) {
        enemy.updateEnemyPosition(playerX, playerY);
    }
}
/*
bool checkPlayerEnemyCollision(const RectPlayer& playerRect, const RectEnemy& enemyRect) {
    if (playerRect.x + playerRect.w <= enemyRect.x || playerRect.x >= enemyRect.x + enemyRect.w ||
        playerRect.y + playerRect.h <= enemyRect.y || playerRect.y >= enemyRect.y + enemyRect.h) {
    }
    else {
        return true;
    }
    return false;
}

void handleCollisions(std::vector<Enemy>& enemies, RectPlayer playerRect) {
    for (int i = enemies.size() - 1; i >= 0; i--) {
        if (checkPlayerEnemyCollision(playerRect, enemies[i].rect)) {
            enemies.erase(enemies.begin() + i);
        }
    }
}*/

void Game::gameLoop() {

    Player player(renderer, screenWidth/2 - (playerWidth/2), screenHeight/2 - (playerHeight/2), playerWidth, playerHeight);
    std::vector<Enemy> enemies;

    while (gameState != GameState::EXIT) {
        handleEvents();
        Sleep(10);

        //Gracz
        player.clearPlayer(renderer, screenWidth, screenHeight);
        player.updatePlayerPosition(screenWidth, screenHeight, playerSpeed);

        SDL_RendererFlip flip = SDL_FLIP_NONE; //obrót gracza
        if (player.facingDirection == Direction::LEFT) {
            flip = SDL_FLIP_HORIZONTAL;
        }

        player.spawnPlayer();

        SDL_Rect playerRect;
        playerRect.x = static_cast<int>(player.rect.x);
        playerRect.y = static_cast<int>(player.rect.y);
        playerRect.w = static_cast<int>(player.rect.w);
        playerRect.h = static_cast<int>(player.rect.h);
        
        SDL_RenderCopyEx(renderer, playerTexture, nullptr, &playerRect, 0, nullptr, flip);

        

        //Wrogowie
        for (Enemy& enemy : enemies) {
            enemy.clearEnemy(renderer);
        }
        generateEnemies(enemies, renderer, screenWidth, screenHeight);
        updateEnemies(enemies, player.rect.x, player.rect.y);
        drawEnemies(enemies);
        //handleCollisions(enemies, player.rect);
        
        
        SDL_RenderPresent(renderer);
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