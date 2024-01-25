#include <iostream>
#include <windows.h>
#include <algorithm>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "game.h"
#include "player.h"
#include "enemy.h"
#include "weapons.h"
#include "render.h"
#include "powerUp.h"



float playerWidth = 50;
float playerHeight = 50;
static int timeBetweenBats = 1250;
static int timeBetweenSkeletons = 4000;
bool isTimeFrozen = false;

SDL_Texture* warriorTexture = nullptr;
SDL_Texture* wizardTexture = nullptr;
SDL_Texture* weaponWhipTexture = nullptr;
SDL_Texture* weaponFireballTexture = nullptr;
SDL_Texture* pauseButtonTexture = nullptr;
SDL_Texture* playButtonTexture = nullptr;

Game::Game()
    : window(nullptr), renderer(nullptr), screenHeight(768), screenWidth(1366), gameState(GameState::PLAY) {
}

Game::~Game() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}

void Game::init(const char* title, int x, int y, int w, int h, Uint32 flags) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cerr << "Nie udalo sie zainicjowac SDL: " << SDL_GetError() << "\n";
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
        std::cerr << "Nie mozna zainicjowac SDL_Image: " << IMG_GetError() << "\n";
    }

}

void Game::loadTextures() {

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

    SDL_Surface* tmpPlaySurface = IMG_Load("assets/play_button.png");
    playButtonTexture = SDL_CreateTextureFromSurface(renderer, tmpPlaySurface);
    SDL_FreeSurface(tmpPlaySurface);
}

void Game::handleStartScreenEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            gameState = GameState::EXIT;
            break;
        case SDL_MOUSEBUTTONDOWN:
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);

            SDL_Rect startButton = { screenWidth / 2 - 120, screenHeight / 2 - 70, 240, 70 };
            if (mouseX >= startButton.x && mouseX <= (startButton.x + startButton.w) &&
                mouseY >= startButton.y && mouseY <= (startButton.y + startButton.h)) {
                startGame = true;
            }

            SDL_Rect exitButton = { screenWidth / 2 - 120, screenHeight / 2 + 30, 240, 70 };
            if (mouseX >= exitButton.x && mouseX <= (exitButton.x + exitButton.w) &&
                mouseY >= exitButton.y && mouseY <= (exitButton.y + exitButton.h)) {
                quitGame = true;
            }
            break;
        }
    }
}

void Game::handleCharacterSelectionEvents(bool& characterSelected) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            gameState = GameState::EXIT;
            break;
        case SDLK_ESCAPE:
            gameState = GameState::EXIT;
            break;
        case SDL_MOUSEBUTTONDOWN:
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);

            SDL_Rect knightButton = { screenWidth / 2 - 270, screenHeight / 2 - 140, 240, 280 };
            if (mouseX >= knightButton.x && mouseX <= (knightButton.x + knightButton.w) &&
                mouseY >= knightButton.y && mouseY <= (knightButton.y + knightButton.h)) {
                selectedCharacter = "warrior";
                characterSelected = true;
            }

            SDL_Rect wizardButton = { screenWidth / 2 + 30, screenHeight / 2 - 140, 240, 280 };
            if (mouseX >= wizardButton.x && mouseX <= (wizardButton.x + wizardButton.w) &&
                mouseY >= wizardButton.y && mouseY <= (wizardButton.y + wizardButton.h)) {
                selectedCharacter = "wizard";
                characterSelected = true;
            }
            break;
        }
    }
}

void Game::handleEndGameEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            gameState = GameState::EXIT;
            break;
        case SDL_MOUSEBUTTONDOWN:
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);

            SDL_Rect restartButton = { screenWidth / 2 - 125, screenHeight / 2 - 100, 250, 50 };
            SDL_Rect characterSelectionButton = { screenWidth / 2 - 125, screenHeight / 2 - 30, 250, 50 };
            SDL_Rect exitButton = { screenWidth / 2 - 125, screenHeight / 2 + 40, 250, 50 };

            //replay
            if (mouseX >= restartButton.x && mouseX <= (restartButton.x + restartButton.w) &&
                mouseY >= restartButton.y && mouseY <= (restartButton.y + restartButton.h)) {
                gameState = GameState::PLAY;
                startGame = true;
                quitGame = false;
                enemiesDefeated = 0;
                batsDefeated = 0;
                skeletonsDefeated = 0;
                timeBetweenBats = 1250;
                timeBetweenSkeletons = 4000;
                loadTextures();
            }

            //wybór postaci
            else if (mouseX >= characterSelectionButton.x && mouseX <= (characterSelectionButton.x + characterSelectionButton.w) &&
                mouseY >= characterSelectionButton.y && mouseY <= (characterSelectionButton.y + characterSelectionButton.h)) {
                gameState = GameState::START_SCREEN;
                characterSelected = false;
                enemiesDefeated = 0;
                batsDefeated = 0;
                skeletonsDefeated = 0;
                timeBetweenBats = 1250;
                timeBetweenSkeletons = 4000;
                loadTextures();
            }

            //wyjście
            else if (mouseX >= exitButton.x && mouseX <= (exitButton.x + exitButton.w) &&
                mouseY >= exitButton.y && mouseY <= (exitButton.y + exitButton.h)) {
                gameState = GameState::EXIT;
            }
            break;
        }
    }
}

void Game::run() {
    init("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);

    Render render(renderer, screenWidth, screenHeight);
    loadTextures();

    while (!quitGame) {
        render.renderStartScreen();
        handleStartScreenEvents();

        if (startGame) {
            while (!characterSelected) {
                render.renderCharacterSelectionScreen(warriorTexture, wizardTexture);
                handleCharacterSelectionEvents(characterSelected);
            }

            while (gameState != GameState::EXIT) {
                if (gameState == GameState::PLAY) {
                    gameLoop();
                }

                if (gameState == GameState::END) {
                    render.renderEndGameScreen(enemiesDefeated);

                    bool endGameHandled = false;
                    while (!endGameHandled) {
                        handleEndGameEvents();

                        if (gameState == GameState::PLAY) {
                            startGame = true;
                            characterSelected = false;
                            endGameHandled = true;
                        }
                        else if (gameState == GameState::START_SCREEN) {
                            while (!characterSelected) {
                                render.renderCharacterSelectionScreen(warriorTexture, wizardTexture);
                                handleCharacterSelectionEvents(characterSelected);
                            }
                            gameState = GameState::PLAY;
                            endGameHandled = true;
                        }
                        else if (gameState == GameState::EXIT) {
                            quitGame = true;
                            endGameHandled = true;
                        }
                    }
                }
            }
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

void generateEnemies(std::vector<std::unique_ptr<Enemy>>& enemies, SDL_Renderer* renderer, int screenWidth, int screenHeight, int& batsDefeated, int& skeletonsDefeated, int& enemiesDefeated) {
    static int lastBatTime = 0;
    static int lastSkeletonTime = 0;

    static int breakpoint = 100;
    static bool batsTimeReduced = false;
    static bool skeletonsTimeReduced = false;

    int currentTime1 = SDL_GetTicks();

    if (currentTime1 > lastBatTime + timeBetweenBats) {
        float randomX = rand() % screenWidth;
        float randomY = rand() % screenHeight;

        std::unique_ptr<Enemy> newBat = std::make_unique<Bat>(renderer, randomX, randomY, 50, 20);
        if (isTimeFrozen) { newBat->setSpeed(0.0f); }
        enemies.push_back(std::move(newBat));

        lastBatTime = currentTime1;

        // Warunek na redukcję czasu co 10 pokonanych nietoperzy
        if (enemiesDefeated <= breakpoint && batsDefeated % 10 == 0 && timeBetweenBats > 100 && !batsTimeReduced) {
            timeBetweenBats -= 100;
            batsTimeReduced = true;
        }

        // Zresetuj zmienną po każdych 10 pokonanych nietoperzach
        if (batsDefeated % 10 != 0) {
            batsTimeReduced = false;
        }
    }

    int currentTime2 = SDL_GetTicks();
    if (enemiesDefeated >= breakpoint && currentTime2 > lastSkeletonTime + timeBetweenSkeletons) {
        float randomX = rand() % screenWidth;
        float randomY = rand() % screenHeight;

        std::unique_ptr<Enemy> newSkeleton = std::make_unique<Skeleton>(renderer, randomX, randomY, 40, 45);
        if (isTimeFrozen) { newSkeleton->setSpeed(0.0f); }
        enemies.push_back(std::move(newSkeleton));

        lastSkeletonTime = currentTime2;

        // Warunek na redukcję czasu co 10 pokonanych szkieletów
        if (skeletonsDefeated % 5 == 0 && skeletonsDefeated < 100 && timeBetweenSkeletons > 1000 && !skeletonsTimeReduced) {
            timeBetweenSkeletons -= 100;
            skeletonsTimeReduced = true;
        }

        // Zresetuj zmienną po każdych 10 pokonanych szkieletach
        if (skeletonsDefeated % 10 != 0) {
            skeletonsTimeReduced = false;
        }
    }
}

void drawEnemies(std::vector<std::unique_ptr<Enemy>>& enemies, float playerX) {
    for (const auto& enemy : enemies) {
        if (dynamic_cast<Bat*>(enemy.get())) {
            dynamic_cast<Bat*>(enemy.get())->draw(playerX);
        }
        else if (dynamic_cast<Skeleton*>(enemy.get())) {
            dynamic_cast<Skeleton*>(enemy.get())->draw(playerX);
        }
    }
}

void updateEnemies(std::vector<std::unique_ptr<Enemy>>& enemies, float playerX, float playerY) {
    for (const auto& enemy : enemies) {
        enemy->updateEnemyPosition(playerX, playerY);
    }
}

bool checkCollision(SDL_Rect rectA, SDL_Rect rectB) {
    if (SDL_HasIntersection(&rectA, &rectB)) return true;
    return false;
}

GameState Game::handleCollisions(std::vector<std::unique_ptr<Enemy>>& enemies, RectPlayer& player, Player& playerObject, Weapon& weapon) {
    for (auto it = enemies.begin(); it != enemies.end();) {
        SDL_Rect playerRect = convertToSDLRect(player);
        SDL_Rect enemyRect = convertToSDLRect((*it)->rect);
        SDL_Rect weaponRect = convertToSDLRect(weapon.rect);

        if (checkCollision(playerRect, enemyRect)) {
            (*it)->isStopped = true;
            playerObject.reduceHealth((*it)->getDamage());
            if (playerObject.getHealth() <= 0) {
                return GameState::END;
            }
        }
        else {
            (*it)->isStopped = false;
        }

        if (selectedCharacter == "warrior") {
            if (checkCollision(weaponRect, enemyRect)) {
                (*it)->reduceHealth(weapon.getDamage());
                if ((*it)->getHealth() <= 0) {
                    if (dynamic_cast<Skeleton*>(it->get()) != nullptr) {
                        enemiesDefeated += 2;
                        skeletonsDefeated++;
                    }
                    else {
                        enemiesDefeated++;
                        batsDefeated++;
                    }
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
        else if (selectedCharacter == "wizard") {
            Fireball* fireball = dynamic_cast<Fireball*>(&weapon);
            bool enemyHit = false;

            for (auto& fb : fireball->fireballs) {
                SDL_Rect fireballRect = convertToSDLRect(fb.rect);

                if (checkCollision(fireballRect, enemyRect)) {
                    (*it)->reduceHealth(fireball->getDamage());
                    enemyHit = true;

                    if ((*it)->getHealth() <= 0) {
                        if (dynamic_cast<Skeleton*>(it->get()) != nullptr) {
                            enemiesDefeated += 2;
                            skeletonsDefeated++;
                        }
                        else {
                            enemiesDefeated++;
                            batsDefeated++;
                        }
                        it = enemies.erase(it);
                    }
                    else {
                        ++it;
                    }
                }
            }

            if (!enemyHit) {
                ++it;
            }
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
            quitGame = true;
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_ESCAPE:
                gameState = GameState::EXIT;
                quitGame = true;
                break;
            case SDLK_p:
                isGamePaused = !isGamePaused;
                break;
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);

            if (mouseX >= screenWidth - 70 && mouseX <= screenWidth && mouseY >= 10 && mouseY <= 70) {
                isGamePaused = !isGamePaused;
            }
            break;
        }
    }
}

void Game::handlePowerUps(Player& player, std::vector<std::unique_ptr<Enemy>>& enemies, std::vector<std::unique_ptr<PowerUp>>& powerUps) {
    // Sprawdź, czy istnieje już power-up danego typu
    bool healthPowerUpExists = std::any_of(powerUps.begin(), powerUps.end(), [](const auto& powerUp) {
        return dynamic_cast<HealthPowerUp*>(powerUp.get()) != nullptr;
        });

    bool timeFreezePowerUpExists = std::any_of(powerUps.begin(), powerUps.end(), [](const auto& powerUp) {
        return dynamic_cast<TimeFreezePowerUp*>(powerUp.get()) != nullptr;
        });

    bool eliminationPowerUpExists = std::any_of(powerUps.begin(), powerUps.end(), [](const auto& powerUp) {
        return dynamic_cast<EliminationPowerUp*>(powerUp.get()) != nullptr;
        });

    //Health
    int currentTime1 = SDL_GetTicks();
    bool enoughTimePassed1 = currentTime1 > timeSinceLastHealthPowerUp + 20000;
    if (!healthPowerUpExists && enoughTimePassed1 && enemiesDefeated > 50) {
        float randomX = rand() % (screenWidth - 30);
        float randomY = rand() % (screenHeight - 30);

        std::unique_ptr<PowerUp> newPowerUp = std::make_unique<HealthPowerUp>(renderer, randomX, randomY, 30, 30);
        powerUps.push_back(std::move(newPowerUp));

        timeSinceLastHealthPowerUp = currentTime1;
    }

    // Time freeze
    int currentTime2 = SDL_GetTicks();
    bool enoughTimePassed2 = currentTime2 > timeSinceLastFreezePowerUp + 30000;
    if (!timeFreezePowerUpExists && enoughTimePassed2 && enemiesDefeated > 100) {
        float randomX = rand() % (screenWidth - 30);
        float randomY = rand() % (screenHeight - 30);

        std::unique_ptr<PowerUp> newPowerUp = std::make_unique<TimeFreezePowerUp>(renderer, randomX, randomY, 30, 30);
        powerUps.push_back(std::move(newPowerUp));

        timeSinceLastFreezePowerUp = currentTime2;
    }

    //Elimination
    int currentTime3 = SDL_GetTicks();
    bool enoughTimePassed3 = currentTime3 > timeSinceLastEliminationPowerUp + 90000;
    if (!eliminationPowerUpExists && enoughTimePassed3 && enemiesDefeated > 200) {
        float randomX = rand() % (screenWidth - 25);
        float randomY = rand() % (screenHeight - 35);

        std::unique_ptr<PowerUp> newPowerUp = std::make_unique<EliminationPowerUp>(renderer, randomX, randomY, 25, 35);
        powerUps.push_back(std::move(newPowerUp));

        timeSinceLastEliminationPowerUp = currentTime3;
    }

    // Obsługa zamrożenia czasu
    if (isTimeFrozen) {
        // Sprawdź, czy upłynął określony czas zamrożenia
        int currentTime = SDL_GetTicks();
        if (currentTime - timeFrozenStartTime >= 10000) { // 10 sekund
            isTimeFrozen = false;

            // Przywróć normalne prędkości po odczekaniu
            for (auto& enemyPtr : enemies) {
                Bat* batEnemy = dynamic_cast<Bat*>(enemyPtr.get());
                Skeleton* skeletonEnemy = dynamic_cast<Skeleton*>(enemyPtr.get());

                if (batEnemy) {
                    batEnemy->setSpeed(1.2f);
                }
                else if (skeletonEnemy) {
                    skeletonEnemy->setSpeed(0.8f);
                }
            }
        }
    }

    // Sprawdź kolizje i działanie
    for (auto it = powerUps.begin(); it != powerUps.end();) {
        SDL_Rect playerRect = convertToSDLRect(player.rect);
        SDL_Rect powerUpRect = convertToSDLRect((*it)->rect);

        if (checkCollision(playerRect, powerUpRect)) {
            (*it)->applyEffect(player, enemies);

            if (dynamic_cast<TimeFreezePowerUp*>((*it).get())) {
                isTimeFrozen = true;
                timeFrozenStartTime = SDL_GetTicks();
                timeSinceLastFreezePowerUp = SDL_GetTicks();
            }
            else if (dynamic_cast<EliminationPowerUp*>((*it).get())) {
                timeSinceLastEliminationPowerUp = SDL_GetTicks();
            }
            else {
                timeSinceLastHealthPowerUp = SDL_GetTicks();
            }

            it = powerUps.erase(it);
            
        }
        else {
            (*it)->draw();
            ++it;
        }
    }
}

void Game::updateGameEntities(Player& player, std::vector<std::unique_ptr<Enemy>>& enemies, Weapon& weapon) {
    // Logika aktualizacji gracza, wrog�w, broni, kolizji itp.
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

    generateEnemies(enemies, renderer, screenWidth, screenHeight, batsDefeated, skeletonsDefeated, enemiesDefeated);
    //std::cout << "timeBetweenBats: " << timeBetweenBats << ", timeBetweenSkeletons: " << timeBetweenSkeletons << std::endl;
    updateEnemies(enemies, player.rect.x, player.rect.y);
}

void Game::renderGame(Player& player, std::vector<std::unique_ptr<Enemy>>& enemies, std::vector<std::unique_ptr<PowerUp>>& powerUps, Weapon& weapon, Render& render) {
    // Renderowanie i czyszczenie tła
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, render.gameScreenTexture, nullptr, nullptr);

    //działanie powerUpów
    handlePowerUps(player, enemies, powerUps);

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
    drawEnemies(enemies, player.rect.x);

    // Renderowanie interfejsu
    render.renderHealth(player.getHealth());
    render.renderScore(enemiesDefeated);
    //render.renderFPS();
    if (isGamePaused) {
        render.renderPauseButton(playButtonTexture);
    }
    else render.renderPauseButton(pauseButtonTexture);

    SDL_RenderPresent(renderer);
}

void Game::gameLoop() {
    Player player(renderer, screenWidth / 2 - (playerWidth / 2), screenHeight / 2 - (playerHeight / 2), playerWidth, playerHeight);
    std::vector<std::unique_ptr<Enemy>> enemies;
    std::vector<std::unique_ptr<PowerUp>> powerUps;
    Render render(renderer, screenWidth, screenHeight);

    Weapon* weapon = nullptr;
    if (selectedCharacter == "warrior") { weapon = new Whip(renderer, player.rect.x + player.rect.w - 120, player.rect.y + (player.rect.h / 2) - 20, 80, 20); }
    else if (selectedCharacter == "wizard") { weapon = new Fireball(renderer, 0, 0, 0, 0); }

    while (gameState == GameState::PLAY) {
        if (!isGamePaused) {
            handleEvents();
            updateGameEntities(player, enemies, *weapon);
            gameState = handleCollisions(enemies, player.rect, player, *weapon);
        }

        handleEvents();
        renderGame(player, enemies, powerUps, *weapon, render);
    }

    delete weapon;
}