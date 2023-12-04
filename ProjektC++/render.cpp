#include <SDL_ttf.h>
#include <iostream>

#include "render.h"

Render::Render(SDL_Renderer* renderer, int screenWidth, int screenHeight)
    : renderer(renderer), screenWidth(screenWidth), screenHeight(screenHeight) {
    if (TTF_Init() == -1) {
        std::cerr << "Nie udalo sie zainicjowac SDL_ttf";
    }

    font = TTF_OpenFont("assets/HeadlinerNo45.ttf", 36);
    textColor = { 36, 36, 36 };
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