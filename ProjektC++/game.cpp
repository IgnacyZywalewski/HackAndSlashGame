#include <windows.h>
#include <vector>
#include <SDL_image.h>

#include "game.h"
#include "player.h"
#include "enemy.h"
#include "weapons.h"
#include "render.h"

float playerWidth = 50;
float playerHeight = 50;
float playerSpeed = 2;
float enemySpeed = 1;

int timeBetweenEnemies = 1000;
int enemiesDefeted = 0;

SDL_Texture* enemyTexture = nullptr;

Game::Game()
    : window(nullptr), renderer(nullptr), screenHeight(768), screenWidth(1360), gameState(GameState::PLAY) {
}

Game::~Game() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
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

    SDL_Surface* tmpSurface2 = IMG_Load("assets/enemy_bat.png");
    enemyTexture = SDL_CreateTextureFromSurface(renderer, tmpSurface2);
    SDL_FreeSurface(tmpSurface2); 
    
}

void Game::run() {
    init("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
    gameLoop();
}

template<typename T>
SDL_Rect convertToSDLRect(const T& rect) {
    SDL_Rect sdlRect;
    sdlRect.x = static_cast<int>(rect.x);
    sdlRect.y = static_cast<int>(rect.y);
    sdlRect.w = static_cast<int>(rect.w);
    sdlRect.h = static_cast<int>(rect.h);
    return sdlRect;
}


void generateEnemies(std::vector<Enemy>& enemies, SDL_Renderer* renderer, int screenWidth, int screenHeight, int& timeBetweenEnemies, int& enemiesDefeated, bool& difficultyIncreased) {
    static int lastEnemyTime = 0;
    int currentTime = SDL_GetTicks();

    if (currentTime > lastEnemyTime + timeBetweenEnemies) {
        float randomX = rand() % screenWidth;
        float randomY = rand() % screenHeight;

        enemies.push_back(Enemy(renderer, randomX, randomY, 50, 20));
        lastEnemyTime = currentTime;
    }

    if (enemiesDefeated % 10 == 0 && enemiesDefeated > 0 && !difficultyIncreased) {
        if (timeBetweenEnemies > 100) { 
            timeBetweenEnemies -= 50;   
            difficultyIncreased = true;
        }
    }

    if (enemiesDefeated % 10 != 0) {
        difficultyIncreased = false;
    }
}




void drawEnemies(std::vector<Enemy>& enemies) {
    for (Enemy& enemy : enemies) {
        enemy.draw(enemyTexture);
    }
}

void updateEnemies(std::vector<Enemy>& enemies, float playerX, float playerY) {
    for (Enemy& enemy : enemies) {
        enemy.updateEnemyPosition(playerX, playerY, enemySpeed);
    }
}
 
bool checkCollision(SDL_Rect rectA, SDL_Rect rectB) {
    if(SDL_HasIntersection(&rectA, &rectB)) return true;
    return false;
}

GameState handleCollisions(std::vector<Enemy>& enemies, RectPlayer& player, Player& playerObject, Weapon& weapon) {
    for (auto it = enemies.begin(); it != enemies.end();) {
        SDL_Rect playerRect = convertToSDLRect(player);
        SDL_Rect enemyRect = convertToSDLRect(it->rect);
        SDL_Rect weaponRect = convertToSDLRect(weapon.rect);

        if (checkCollision(playerRect, enemyRect)) {
            it->isStopped = true;
            playerObject.reduceHealth(it->getDamage());
            if (playerObject.getHealth() <= 0) {
                return GameState::EXIT;
            }
        }
        else {
            it->isStopped = false;
        }

        if (checkCollision(weaponRect, enemyRect)) {
            it->reduceHealth(weapon.getDamage());
            if (it->getHealth() <= 0) {
                it = enemies.erase(it);
                enemiesDefeted++;
            }
            else {
                ++it;
            }
        }
        else {
            ++it;
        }
    }
    return GameState::PLAY;
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

void Game::updateGameEntities(Player& player, std::vector<Enemy>& enemies, Weapon& weapon) {
    // Logika aktualizacji gracza, wrogów, broni, kolizji itp.
    Sleep(10);
    player.updatePlayerPosition(screenWidth, screenHeight, playerSpeed);
    weapon.updatePosition(player.rect.x, player.rect.y, player.rect.w, player.rect.h);
    generateEnemies(enemies, renderer, screenWidth, screenHeight, timeBetweenEnemies, enemiesDefeted, player.flag);
    updateEnemies(enemies, player.rect.x, player.rect.y);
}

void Game::renderGame(Player& player, std::vector<Enemy>& enemies, Weapon& weapon, Render& render) {
    // Renderowanie i czyszczenie t³a
    SDL_SetRenderDrawColor(renderer, 169, 169, 169, 255);
    SDL_RenderClear(renderer);

    // Renderowanie interfejsu
    render.renderHealth(player.getHealth());
    render.renderScore(enemiesDefeted);
    render.renderFPS();

    // Renderowanie gracza, wrogów, broni.
    player.spawnPlayer();
    weapon.drawWeapon(renderer);
    drawEnemies(enemies);

    // Wyœwietlanie na ekranie
    SDL_RenderPresent(renderer);
}

void Game::gameLoop() {
    Player player(renderer, screenWidth / 2 - (playerWidth / 2), screenHeight / 2 - (playerHeight / 2), playerWidth, playerHeight);
    std::vector<Enemy> enemies;
    Weapon weapon(renderer, player.rect.x + player.rect.w, (player.rect.y + player.rect.h) / 2, 80, 30);
    Render render(renderer, screenWidth, screenHeight);

    while (gameState != GameState::EXIT) {
        updateGameEntities(player, enemies, weapon);
        gameState = handleCollisions(enemies, player.rect, player, weapon);
        renderGame(player, enemies, weapon, render);
        handleEvents();
    }
}