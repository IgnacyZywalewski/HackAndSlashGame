#include <windows.h>
#include <vector>
#include <SDL_image.h>

#include "game.h"
#include "player.h"
#include "enemy.h"
#include "weapons.h"

float playerWidth = 50;
float playerHeight = 50;
float playerSpeed = 3.0;
float enemySpeed = 1.0;

int timeBetweenEnemies = 60; //60 klatek = 1 sekunda

SDL_Texture* enemyTexture = nullptr;

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

    SDL_Surface* tmpSurface2 = IMG_Load("assets/enemy_bat.png");
    enemyTexture = SDL_CreateTextureFromSurface(renderer, tmpSurface2);
    SDL_FreeSurface(tmpSurface2); 
    
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


void generateEnemies(std::vector<Enemy>& enemies, SDL_Renderer* renderer, int screenWidth, int screenHeight, int time) {
    static int timer = 0;
    timer++;
    if (timer >= time) {
        float randomX = rand() % screenWidth;
        float randomY = rand() % screenHeight;
        
        enemies.push_back(Enemy(renderer, randomX, randomY, 50, 20));
               
        timer = 0;
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

void Game::gameLoop() {
    Player player(renderer, screenWidth / 2 - (playerWidth / 2), screenHeight / 2 - (playerHeight / 2), playerWidth, playerHeight);
    std::vector<Enemy> enemies;
    Weapon weapon(renderer, player.rect.x + player.rect.w, (player.rect.y + player.rect.h) / 2, 80, 30);

    while (gameState != GameState::EXIT) {
        handleEvents();
        Sleep(10);

        //Ustawienie i czyszczenie t³a
        SDL_SetRenderDrawColor(renderer, 169, 169, 169, 255);
        SDL_RenderClear(renderer);

        //Gracz
        player.updatePlayerPosition(screenWidth, screenHeight, playerSpeed);
        player.spawnPlayer();

        //Broñ
        weapon.updatePosition(player.rect.x, player.rect.y, player.rect.w, player.rect.h);
        weapon.drawWeapon(renderer);

        //Wrogowie
        generateEnemies(enemies, renderer, screenWidth, screenHeight, timeBetweenEnemies);
        updateEnemies(enemies, player.rect.x, player.rect.y);
        drawEnemies(enemies);

        // Obs³uga kolizji
        gameState = handleCollisions(enemies, player.rect, player, weapon);
        

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
