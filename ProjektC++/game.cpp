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
int enemiesDefeated = 0;

SDL_Texture* enemyTexture = nullptr;
SDL_Texture* warriorTexture = nullptr;
SDL_Texture* wizardTexture = nullptr;
SDL_Texture* weaponWhipTexture = nullptr;
SDL_Texture* weaponFireballTexture = nullptr;
SDL_Texture* pauseButtonTexture = nullptr;

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

    SDL_Surface* tmpKnightSurface = IMG_Load("assets/player_warrior.png");
    warriorTexture = SDL_CreateTextureFromSurface(renderer, tmpKnightSurface);
    SDL_FreeSurface(tmpKnightSurface);

    SDL_Surface* tmpWizardSurface = IMG_Load("assets/player_wizard.png");
    wizardTexture = SDL_CreateTextureFromSurface(renderer, tmpWizardSurface);
    SDL_FreeSurface(tmpWizardSurface);

    SDL_Surface* tmpWhipSurface = IMG_Load("assets/weapon_whip.png");
    weaponWhipTexture = SDL_CreateTextureFromSurface(renderer, tmpWhipSurface);
    SDL_FreeSurface(tmpWhipSurface);

    SDL_Surface* tmpFireballSurface = IMG_Load("assets/weapon_fireball.png");
    weaponFireballTexture = SDL_CreateTextureFromSurface(renderer, tmpFireballSurface);
    SDL_FreeSurface(tmpFireballSurface);

    SDL_Surface* tmpPauseSurface = IMG_Load("assets/pause_button.png");
    pauseButtonTexture = SDL_CreateTextureFromSurface(renderer, tmpPauseSurface);
    SDL_FreeSurface(tmpPauseSurface);
    
}

void Game::run() {
    init("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
    bool characterSelected = false;

    while (!characterSelected) {
        renderCharacterSelectionScreen();
        handleCharacterSelectionEvents(characterSelected);
    }

    gameState = GameState::PLAY;
    gameLoop();
}

void Game::renderCharacterSelectionScreen() {
    SDL_SetRenderDrawColor(renderer, 169, 169, 169, 169);
    SDL_RenderClear(renderer);

    SDL_Rect knightButton = { screenWidth/2 - 250, screenHeight/2 - 100, 200, 200 };
    SDL_Rect wizardButton = { screenWidth/2 + 50, screenHeight/2 - 100, 200, 200 };

    SDL_RenderCopy(renderer, warriorTexture, nullptr, &knightButton);
    SDL_RenderCopy(renderer, wizardTexture, nullptr, &wizardButton);

    SDL_RenderPresent(renderer);
}

void Game::handleCharacterSelectionEvents(bool& characterSelected) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            gameState = GameState::EXIT;
            break;
        case SDL_MOUSEBUTTONDOWN:
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);

            SDL_Rect knightButton = { screenWidth / 2 - 250, screenHeight / 2 - 100, 200, 200 };
            if (mouseX >= knightButton.x && mouseX <= (knightButton.x + knightButton.w) &&
                mouseY >= knightButton.y && mouseY <= (knightButton.y + knightButton.h)) {
                selectedCharacter = "warrior";
                characterSelected = true;
            }

            SDL_Rect wizardButton = { screenWidth / 2 + 50, screenHeight / 2 - 100, 200, 200 };
            if (mouseX >= wizardButton.x && mouseX <= (wizardButton.x + wizardButton.w) &&
                mouseY >= wizardButton.y && mouseY <= (wizardButton.y + wizardButton.h)) {
                selectedCharacter = "wizard";
                characterSelected = true;
            }
            break;
        }
    }
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
                enemiesDefeated++;
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
    while (SDL_PollEvent(&event)) {
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
        case SDL_MOUSEBUTTONDOWN:
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);

            if (mouseX >= screenWidth - 60 && mouseX <= screenWidth && mouseY >= 10 && mouseY <= 90) {
                isGamePaused = !isGamePaused;
            }
            break;
        }
    }
}
void Game::updateGameEntities(Player& player, std::vector<Enemy>& enemies, Weapon& weapon) {
    // Logika aktualizacji gracza, wrogów, broni, kolizji itp.
    Sleep(10);
    player.updatePlayerPosition(screenWidth, screenHeight, playerSpeed);

    if (selectedCharacter == "warrior") {
        Whip* whip = dynamic_cast<Whip*>(&weapon);
        if (whip) {
            whip->updatePosition(player.rect.x, player.rect.y, player.rect.w, player.rect.h);
        }
    }
    else if (selectedCharacter == "wizard") {
        Fireball* fireball = dynamic_cast<Fireball*>(&weapon);
        if (fireball) {
            fireball->updatePosition(player.rect.x, player.rect.y, player.rect.w, player.rect.h);
        }
    }

    generateEnemies(enemies, renderer, screenWidth, screenHeight, timeBetweenEnemies, enemiesDefeated, player.flag);
    updateEnemies(enemies, player.rect.x, player.rect.y);
}

void Game::renderGame(Player& player, std::vector<Enemy>& enemies, Weapon& weapon, Render& render) {
    // Renderowanie i czyszczenie t³a
    SDL_SetRenderDrawColor(renderer, 169, 169, 169, 169);
    SDL_RenderClear(renderer);

    // Renderowanie interfejsu
    render.renderHealth(player.getHealth());
    render.renderScore(enemiesDefeated);
    render.renderFPS();
    render.renderPauseButton(pauseButtonTexture);

    // Renderowanie gracza, wrogów, broni.
    player.spawnPlayer();
    if (selectedCharacter == "warrior") {
        player.playerTexture = warriorTexture;
        Whip* whip = dynamic_cast<Whip*>(&weapon);
        if (whip) {
            whip->weaponTexture = weaponWhipTexture;
            whip->drawWeapon(renderer);
        }
    }
    else if (selectedCharacter == "wizard") {
        player.playerTexture = wizardTexture;
        Fireball* fireball = dynamic_cast<Fireball*>(&weapon);
        if (fireball) {
            fireball->weaponTexture = weaponFireballTexture;
            fireball->drawWeapon(renderer);
        }
    }

    drawEnemies(enemies);
    SDL_RenderPresent(renderer);
}

void Game::gameLoop() {
    Player player(renderer, screenWidth / 2 - (playerWidth / 2), screenHeight / 2 - (playerHeight / 2), playerWidth, playerHeight);
    std::vector<Enemy> enemies;
    Render render(renderer, screenWidth, screenHeight);

    // Tworzenie broni w zale¿noœci od wybranej postaci
    Weapon* weapon = nullptr;

    if (selectedCharacter == "warrior") {
        weapon = new Whip(renderer, player.rect.x + player.rect.w - 120, player.rect.y + (player.rect.h / 2) - 20, 80, 20);
    }
    else if (selectedCharacter == "wizard") {
        weapon = new Fireball(renderer, 0, 0, 0, 0);
    }

    while (gameState != GameState::EXIT) {
        handleEvents();

        if (!isGamePaused) {
            updateGameEntities(player, enemies, *weapon);
            gameState = handleCollisions(enemies, player.rect, player, *weapon);
        }

        renderGame(player, enemies, *weapon, render);
    }

    delete weapon;
}
