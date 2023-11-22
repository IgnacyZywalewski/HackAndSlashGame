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

int timeBetweenEnemies = 180; //60 klatek = 1 sekunda

SDL_Texture* playerTexture = nullptr;
SDL_Texture* enemyTexture = nullptr;
//SDL_Texture* weaponTexture = nullptr;
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

    SDL_Surface* tmpSurface = IMG_Load("assets/player.png");
    playerTexture = SDL_CreateTextureFromSurface(renderer, tmpSurface);
    SDL_FreeSurface(tmpSurface);

    SDL_Surface* tmpSurface2 = IMG_Load("assets/enemy_bat.png");
    enemyTexture = SDL_CreateTextureFromSurface(renderer, tmpSurface2);
    SDL_FreeSurface(tmpSurface2); 
    
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
 

bool checkPlayerEnemyCollision(const RectPlayer& playerRect, const RectEnemy& enemyRect) {
    if (playerRect.x + playerRect.w <= enemyRect.x ||
        playerRect.x >= enemyRect.x + enemyRect.w ||
        playerRect.y + playerRect.h <= enemyRect.y || 
        playerRect.y >= enemyRect.y + enemyRect.h ) {
        return false;
    }
    else {
        return true;
    }
}

/*
bool checkWeaponEnemyCollision(const RectWeapon& weaponRect, const RectEnemy& enemyRect) {
    if (weaponRect.x + weaponRect.w <= enemyRect.x ||
        weaponRect.x >= enemyRect.x + enemyRect.w ||
        weaponRect.y + weaponRect.h <= enemyRect.y ||
        weaponRect.y >= enemyRect.y + enemyRect.h) {
        return false;
    }
    else {
        return true;
    }
}*/


GameState handleCollisions(std::vector<Enemy>& enemies, RectPlayer& playerRect, Player& player) {
    for (auto it = enemies.begin(); it != enemies.end(); it++) {
        if (checkPlayerEnemyCollision(playerRect, it->rect)) {
            it->isStopped = true;

            player.reduceHealth(it->getDamage());
            if (player.getHealth() <= 0) {
                return GameState::EXIT;
            }
        }
        else {
            it->isStopped = false;
        }

        /*
        if (checkWeaponEnemyCollision(weapon.rect, it->rect)) {
            it->reduceHealth(weapon.getDamage());
            if (it->getHealth() <= 0) {
                it = enemies.erase(it);
            }
        }*/
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

void Game::gameLoop() {

    Player player(renderer, screenWidth/2 - (playerWidth/2), screenHeight/2 - (playerHeight/2), playerWidth, playerHeight);
    std::vector<Enemy> enemies;
    //Weapon weapon(renderer, player.rect.x + player.rect.w, (player.rect.y + player.rect.h)/2, 80, 30);

    while (gameState != GameState::EXIT) {
        handleEvents();
        Sleep(10);

        //Ustawienie i czyszczenie t³a
        SDL_SetRenderDrawColor(renderer, 169, 169, 169, 255);
        SDL_RenderClear(renderer);

        //Gracz
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
        generateEnemies(enemies, renderer, screenWidth, screenHeight, timeBetweenEnemies);
        updateEnemies(enemies, player.rect.x, player.rect.y);
        drawEnemies(enemies);
        gameState = handleCollisions(enemies, player.rect, player);
        
        
        SDL_RenderPresent(renderer);
    }
}