#include "Game.hpp"
#include <cstdlib>
#include <ctime>
#include <string>

bool Game::Initialise() {
    int sdlResult = SDL_Init(SDL_INIT_VIDEO);
    if (sdlResult != 0) {
        SDL_Log("Unable to initialise SDL: %s", SDL_GetError());
        return false;
    }

    TTF_Init();

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

    mBallVelocity = { -200.0f, randomY() };
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
    mIsPaused = true;
    mScore = 0;
    mNextScore = 10;

    mFont = TTF_OpenFont("resources/ARCADECLASSIC.TTF", 25);

    return true;
}

void Game::ShutDown() {
    TTF_CloseFont(mFont);
    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);
    TTF_Quit();
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
        case SDL_KEYUP:
            if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
                mIsPaused = !mIsPaused;
            }
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

    if (mIsPaused) {
        return;
    }

    movePaddle(deltaTime);
    checkCollisions();
    handleHit();
    handleMiss();

    mBallPosition.x += mBallVelocity.x * deltaTime;
    mBallPosition.y += mBallVelocity.y * deltaTime;
};

void Game::movePaddle(const float& deltaTime) {
    if (mPaddleDir != 0) {
        mPaddlePosition.y += mPaddleDir * mPaddleVelocity * deltaTime;

        if (mPaddlePosition.y < mPaddleHeight / 2) {
            mPaddlePosition.y = mPaddleHeight / 2;
        }

        if (mPaddlePosition.y > mHeight - (mPaddleHeight / 2)) {
            mPaddlePosition.y = mHeight - (mPaddleHeight / 2);
        }
    }
}

void Game::checkCollisions() {
    if (mBallPosition.y <= mThickness && mBallVelocity.y < 0.0f) {
        mBallVelocity.y *= -1;
    }

    if (mBallPosition.y >= mHeight - mThickness && mBallVelocity.y > 0.0f) {
        mBallVelocity.y *= -1;
    }

    if (mBallPosition.x >= mWidth - mThickness && mBallVelocity.x > 0.0f) {
        mBallVelocity.x *= -1;
    }
}

void Game::handleHit() {
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

                mBallVelocity.y = randomY();

                mScore += mNextScore;
                mNextScore += 10;
            }
        }
    }
}

void Game::handleMiss() {
    if (mBallPosition.x <= 0) {
        if (mBallPosition.y < mPaddlePosition.y - (mPaddleHeight / 2)
            || mBallPosition.y > mPaddlePosition.y + (mPaddleHeight / 2)) {
            mIsPaused = true;
            mBallPosition = {
                static_cast<int>(mWidth / 2),
                static_cast<int>(mHeight / 2)
            };
            mBallVelocity = { -200.0f, randomY() };

            mScore -= 50;
            if (mScore < 0) {
                mScore = 0;
            }
            mNextScore = 10;

            return;
        }
    }
}

void Game::GenerateOutput() {
    SDL_SetRenderDrawColor(mRenderer, 0, 0, 255, 255);
    SDL_RenderClear(mRenderer);

    SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);

    SDL_Rect topWall {
        0, 0, mWidth, mThickness
    };
    SDL_Rect bottomWall {
        0, mHeight - mThickness, mWidth, mThickness
    };
    SDL_Rect backWall {
        mWidth - mThickness, 0, mThickness, mHeight
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

    SDL_Colour scoreColour = { 180, 180, 180 };
    SDL_Surface* scoreSurface = TTF_RenderText_Solid(mFont, std::to_string(mScore).c_str(), scoreColour);
    SDL_Texture* scoreTexture = SDL_CreateTextureFromSurface(mRenderer, scoreSurface);

    int scoreWidth { 0 };
    int scoreHeight { 0 };
    SDL_QueryTexture(scoreTexture, nullptr, nullptr, &scoreWidth, &scoreHeight);
    SDL_Rect scoreRect = { mWidth - mThickness * 2 - scoreWidth, mThickness * 2, scoreWidth, scoreHeight };

    SDL_RenderCopy(mRenderer, scoreTexture, nullptr, &scoreRect);

    SDL_DestroyTexture(scoreTexture);
    SDL_FreeSurface(scoreSurface);

    SDL_RenderPresent(mRenderer);
};

float Game::randomY() {
    std::srand(std::time(nullptr));

    float magnitude = (float)(std::rand() % 100);
    int direction = std::rand() % 2 == 1 ? 1 : -1;

    return (160 + magnitude) * direction;
}
