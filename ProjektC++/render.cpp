#include <SDL_ttf.h>
#include <SDL_image.h>
#include <iostream>

#include "render.h"

Render::Render(SDL_Renderer* renderer, int screenWidth, int screenHeight)
    : renderer(renderer), screenWidth(screenWidth), screenHeight(screenHeight) {
    if (TTF_Init() == -1) {
        std::cerr << "Nie udalo sie zainicjowac SDL_ttf";
    }

    font = TTF_OpenFont("assets/HeadlinerNo45.ttf", 36);
    textColor = { 36, 36, 36 };

    SDL_Surface* tmpScreenSurface = IMG_Load("assets/start_screen.png");
    screenTexture = SDL_CreateTextureFromSurface(renderer, tmpScreenSurface);
    SDL_FreeSurface(tmpScreenSurface);

    SDL_Surface* tmpButtonSurface = IMG_Load("assets/button.png");
    buttonTexture = SDL_CreateTextureFromSurface(renderer, tmpButtonSurface);
    SDL_FreeSurface(tmpButtonSurface);

    SDL_Surface* tmpGameScreenSurface = IMG_Load("assets/game_screen.png");
    gameScreenTexture = SDL_CreateTextureFromSurface(renderer, tmpGameScreenSurface);
    SDL_FreeSurface(tmpGameScreenSurface);

    SDL_Surface* tmpFrameSurface = IMG_Load("assets/frame.png");
    frameTexture = SDL_CreateTextureFromSurface(renderer, tmpFrameSurface);
    SDL_FreeSurface(tmpFrameSurface);

}

Render::~Render() {
    TTF_CloseFont(font);
    TTF_Quit();
}

void Render::renderHealth(int health) {
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, ("Health : " + std::to_string(health)).c_str(), textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    SDL_Rect textRect = { 10, 10, textSurface->w, textSurface->h };
    SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}

void Render::renderScore(int score) {
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, ("Score : " + std::to_string(score)).c_str(), textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    SDL_Rect textRect = { 10, 45, textSurface->w, textSurface->h };
    SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}

void Render::renderFPS() {
    static Uint32 lastTime = 0;
    static int frames = 0;
    static std::string fpsText = "FPS: 0";

    Uint32 currentTime = SDL_GetTicks();
    frames++;

    if (currentTime - lastTime >= 1000) {
        lastTime = currentTime;
        fpsText = "FPS: " + std::to_string(frames);
        frames = 0;
    }

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, fpsText.c_str(), textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    SDL_Rect textRect = { 10, screenHeight - 40, textSurface->w, textSurface->h };
    SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);

    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);
}

void Render::renderPauseButton(SDL_Texture* pauseTexture) {
    SDL_Rect pauseButtonRect;
    pauseButtonRect.x = screenWidth - 60;
    pauseButtonRect.y = 15;
    pauseButtonRect.w = 40;
    pauseButtonRect.h = 45;

    SDL_RenderCopy(renderer, pauseTexture, nullptr, &pauseButtonRect);
}

void Render::renderStartScreen() {
    SDL_RenderClear(renderer);

    //t³o
    SDL_RenderCopy(renderer, screenTexture, nullptr, nullptr);

    //przyciski
    SDL_Rect startButton = { screenWidth / 2 - 120, screenHeight / 2 - 70, 240, 70 };
    SDL_Rect exitButton = { screenWidth / 2 - 120, screenHeight / 2 + 30, 240, 70 };

    SDL_RenderCopy(renderer, buttonTexture, nullptr, &startButton);
    SDL_RenderCopy(renderer, buttonTexture, nullptr, &exitButton);

    //napisy na przyciskach
    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, "Start", textColor);
    SDL_Texture* textStart = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
    SDL_Rect startTextRect = { screenWidth / 2 - 50, screenHeight / 2 - 50, 100, 35 };
    SDL_RenderCopy(renderer, textStart, nullptr, &startTextRect);

    surfaceMessage = TTF_RenderText_Solid(font, "Exit", textColor);
    SDL_Texture* textExit = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
    SDL_Rect exitTextRect = { screenWidth / 2 - 40, screenHeight / 2 + 50, 80, 35 };
    SDL_RenderCopy(renderer, textExit, nullptr, &exitTextRect);

    SDL_FreeSurface(surfaceMessage);
    SDL_DestroyTexture(textStart);
    SDL_DestroyTexture(textExit);

    SDL_RenderPresent(renderer);
}

void Render::renderCharacterSelectionScreen(SDL_Texture* warriorTexture, SDL_Texture* wizardTexture) {
    SDL_RenderClear(renderer);

    //tekstura t³a
    SDL_RenderCopy(renderer, screenTexture, nullptr, nullptr);

    //napis na górze
    std::string topText = "Choose your charackter";
    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, topText.c_str(), textColor);
    SDL_Texture* topTextTexture = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
    SDL_Rect topTextRect = { screenWidth / 2 - 250, screenHeight - 650, 480, 60 };
    SDL_RenderCopy(renderer, topTextTexture, nullptr, &topTextRect);

    //kwadraty
    SDL_Rect knightBackground = { screenWidth / 2 - 290, screenHeight / 2 - 160, 280, 320 };
    SDL_Rect wizardBackground = { screenWidth / 2 + 10, screenHeight / 2 - 160, 280, 320 };

    //szare t³o
    SDL_SetRenderDrawColor(renderer, 169, 169, 169, 169);
    SDL_RenderFillRect(renderer, &knightBackground);
    SDL_RenderFillRect(renderer, &wizardBackground);

    //ramki
    SDL_RenderCopy(renderer, frameTexture, nullptr, &knightBackground);
    SDL_RenderCopy(renderer, frameTexture, nullptr, &wizardBackground);

    //tekstury postaci
    SDL_Rect knightButton = { screenWidth / 2 - 250, screenHeight / 2 - 100, 200, 200 };
    SDL_Rect wizardButton = { screenWidth / 2 + 50, screenHeight / 2 - 100, 200, 200 };
    SDL_RenderCopy(renderer, warriorTexture, nullptr, &knightButton);
    SDL_RenderCopy(renderer, wizardTexture, nullptr, &wizardButton);

    //napisy pod teksturami
    SDL_Color textColor = {255, 255, 255, 255};

    std::string warriorText = "Warrior";
    surfaceMessage = TTF_RenderText_Solid(font, warriorText.c_str(), textColor);
    SDL_Texture* warriorTextTexture = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
    SDL_Rect warriorTextRect = { screenWidth / 2 - 230, screenHeight / 2 + 170, 160, 30 };
    SDL_RenderCopy(renderer, warriorTextTexture, nullptr, &warriorTextRect);

    std::string wizardText = "Wizard";
    surfaceMessage = TTF_RenderText_Solid(font, wizardText.c_str(), textColor);
    SDL_Texture* wizardTextTexture = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
    SDL_Rect wizardTextRect = { screenWidth / 2 + 80, screenHeight / 2 + 170, 130, 30 };
    SDL_RenderCopy(renderer, wizardTextTexture, nullptr, &wizardTextRect);

    //zwolnienie pamiêci
    SDL_FreeSurface(surfaceMessage);
    SDL_DestroyTexture(topTextTexture);

    SDL_RenderPresent(renderer);
}

void Render::renderEndGameScreen(int enemiesDefeated) {
    SDL_RenderClear(renderer);

    //t³o
    SDL_RenderCopy(renderer, screenTexture, nullptr, nullptr);

    //przyciski
    SDL_Rect restartButton = { screenWidth / 2 - 125, screenHeight / 2 - 110, 250, 70 };
    SDL_Rect characterSelectionButton = { screenWidth / 2 - 125, screenHeight / 2 - 30, 250, 70 };
    SDL_Rect exitButton = { screenWidth / 2 - 125, screenHeight / 2 + 50, 250, 70 };

    SDL_RenderCopy(renderer, buttonTexture, nullptr, &restartButton);
    SDL_RenderCopy(renderer, buttonTexture, nullptr, &characterSelectionButton);
    SDL_RenderCopy(renderer, buttonTexture, nullptr, &exitButton);

    //wynik
    std::string scoreText = "Your score: " + std::to_string(enemiesDefeated);
    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, scoreText.c_str(), textColor);
    SDL_Texture* scoreTexture = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
    SDL_Rect scoreTextRect = { screenWidth / 2 - 150, screenHeight - 600, 300, 50 };
    SDL_RenderCopy(renderer, scoreTexture, nullptr, &scoreTextRect);

    //napisy na przyciskach 
    surfaceMessage = TTF_RenderText_Solid(font, "Replay", textColor);
    SDL_Texture* textStart = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
    SDL_Rect startTextRect = { screenWidth / 2 - 45, screenHeight / 2 - 90, 90, 35 };
    SDL_RenderCopy(renderer, textStart, nullptr, &startTextRect);

    surfaceMessage = TTF_RenderText_Solid(font, "Choose charackter", textColor);
    SDL_Texture* textCharckter = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
    SDL_Rect charTextRect = { screenWidth / 2 - 93, screenHeight / 2 - 10, 190, 35 };
    SDL_RenderCopy(renderer, textCharckter, nullptr, &charTextRect);

    surfaceMessage = TTF_RenderText_Solid(font, "Exit", textColor);
    SDL_Texture* textExit = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
    SDL_Rect exitTextRect = { screenWidth / 2 - 35, screenHeight / 2 + 70, 70, 35 };
    SDL_RenderCopy(renderer, textExit, nullptr, &exitTextRect);


    SDL_FreeSurface(surfaceMessage);
    SDL_DestroyTexture(textCharckter);
    SDL_DestroyTexture(scoreTexture);
    SDL_DestroyTexture(textStart);
    SDL_DestroyTexture(textExit);

    SDL_RenderPresent(renderer);
}