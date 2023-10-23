#include "game.h"
#include "player.h"

Game::Game()
    : window(nullptr), renderer(nullptr), screenHeight(600), screenWidth(1024), gameState(GameState::PLAY) {
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

void Game::gameLoop() {
    while (gameState != GameState::EXIT) {
        
        handleEvents();

        Player player(renderer, 0, 0, 100, 100);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        player.render();

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
    }
}