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

    SDL_GetWindowSize(mWindow, &mWidth, &mHeight);
    mBallPosition = {
        static_cast<int>(mWidth / 2),
        static_cast<int>(mHeight / 2)
    };
    mBallVelocity = { -200.0f, 235.0f };
    mThickness = 16;
    mPaddleHeight = 128;
    mPaddlePosition = {
        mThickness,
        static_cast<int>(mHeight / 2)
    };
    mPaddleVelocity = 300.0f;
    mTicksCount = 0;
    frameRate = 32;
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
    mPaddleDir = 0;
    if (state[SDL_SCANCODE_ESCAPE]) {
        mIsRunning = false;
    }
    if (state[SDL_SCANCODE_W]) {
        mPaddleDir -= 1;
    }
    if (state[SDL_SCANCODE_S]) {
        mPaddleDir += 1;
    }
};

void Game::UpdateGame() {
    auto ticks = SDL_GetTicks();
    if (ticks - mTicksCount < frameRate) {
        SDL_Delay(ticks - mTicksCount);
    }

    ticks = SDL_GetTicks();

    float deltaTime = (ticks - mTicksCount) / 1000.0f;
    if (deltaTime > 0.05f) {
        deltaTime = 0.05f;
    }

    mTicksCount = ticks;

    if (mPaddleDir != 0) {
        mPaddlePosition.y += mPaddleDir * mPaddleVelocity * deltaTime;

        if (mPaddlePosition.y < mPaddleHeight / 2) {
            mPaddlePosition.y = mPaddleHeight / 2;
        }

        if (mPaddlePosition.y > mHeight - (mPaddleHeight / 2)) {
            mPaddlePosition.y = mHeight - (mPaddleHeight / 2);
        }
    }

    if (mBallPosition.y <= mThickness && mBallVelocity.y < 0.0f) {
        mBallVelocity.y *= -1;
    }

    if (mBallPosition.y >= mHeight - mThickness && mBallVelocity.y > 0.0f) {
        mBallVelocity.y *= -1;
    }

    if (mBallPosition.x >= mWidth - mThickness && mBallVelocity.x > 0.0f) {
        mBallVelocity.x *= -1;
    }

    if (mBallPosition.x <= mPaddlePosition.x + mThickness) {
        if (mBallPosition.y >= mPaddlePosition.y - (mPaddleHeight / 2)
            && mBallPosition.y <= mPaddlePosition.y + (mPaddleHeight / 2)) {
            if (mBallVelocity.x < 0.0f) {
                mBallVelocity.x *= -1;

                if (mBallVelocity.x < 0.0f) {
                    mBallVelocity.x -= 20.0f;
                } else {
                    mBallVelocity.x += 20.0f;
                }
            }
        }
    }

    mBallPosition.x += mBallVelocity.x * deltaTime;
    mBallPosition.y += mBallVelocity.y * deltaTime;
};

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
    SDL_Rect backWall {
        1024 - mThickness, 0, mThickness, 768
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
    SDL_RenderFillRect(mRenderer, &backWall);
    SDL_RenderFillRect(mRenderer, &ball);
    SDL_RenderFillRect(mRenderer, &paddle);

    SDL_RenderPresent(mRenderer);
};
