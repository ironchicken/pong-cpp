#include "Game.hpp"

bool Game::Initialise() {
    int sdlResult = SDL_Init(SDL_INIT_VIDEO);
    if (sdlResult != 0) {
        SDL_Log("Unable to initialise SDL: %s", SDL_GetError());
        return false;
    }

    mWindow = SDL_CreateWindow("Pong", 100, 100, 1024, 768, 0);

    if (mWindow == nullptr) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }

    return true;
}

void Game::ShutDown() {
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
}

void Game::RunLoop() {
    while (mIsRunning) {
        ProcessInput();
        UpdateGame();
        GenerateOutput();
    }
}

void Game::ProcessInput() {};
void Game::UpdateGame() {};
void Game::GenerateOutput() {};
