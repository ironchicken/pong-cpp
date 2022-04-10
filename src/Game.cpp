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

    mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (mRenderer == nullptr) {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        return false;
    }

    int w;
    int h;
    SDL_GetWindowSize(mWindow, &w, &h);
    mBallPosition = {
        static_cast<int>(w / 2),
        static_cast<int>(h / 2)
    };
    mThickness = 16;
    mPaddleHeight = 128;
    mPaddlePosition = {
        mThickness,
        static_cast<int>(h / 2)
    };
    mIsRunning = true;

    return true;
}

void Game::ShutDown() {
    SDL_DestroyRenderer(mRenderer);
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

void Game::ProcessInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            mIsRunning = false;
            break;
        }
    }

    const Uint8* state = SDL_GetKeyboardState(nullptr);
    if (state[SDL_SCANCODE_ESCAPE]) {
        mIsRunning = false;
    }
};

void Game::UpdateGame() {};

void Game::GenerateOutput() {
    SDL_SetRenderDrawColor(mRenderer, 0, 0, 255, 255);
    SDL_RenderClear(mRenderer);

    SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);

    SDL_Rect topWall {
        0, 0, 1024, mThickness
    };
    SDL_Rect bottomWall {
        0, 768 - mThickness, 1024, mThickness
    };

    SDL_Rect ball {
        static_cast<int>(mBallPosition.x - mThickness / 2),
        static_cast<int>(mBallPosition.y - mThickness / 2),
        mThickness,
        mThickness
    };

    SDL_Rect paddle {
        static_cast<int>(mPaddlePosition.x),
        static_cast<int>(mPaddlePosition.y - mPaddleHeight / 2),
        mThickness,
        mPaddleHeight
    };

    SDL_RenderFillRect(mRenderer, &topWall);
    SDL_RenderFillRect(mRenderer, &bottomWall);
    SDL_RenderFillRect(mRenderer, &ball);
    SDL_RenderFillRect(mRenderer, &paddle);

    SDL_RenderPresent(mRenderer);
};
