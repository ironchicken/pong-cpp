#ifndef GAME_H
#define GAME_H

#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

template <typename T>
struct Vector2 {
    T x;
    T y;
};

class Game {
public:
    Game() {};
    bool Initialise();
    void RunLoop();
    void ShutDown();

private:
    void ProcessInput();
    void UpdateGame();
    void GenerateOutput();
    float randomY();
    void movePaddle(const float& deltaTime);
    void checkCollisions();
    void handleHit();
    void handleMiss();
    void moveBall(const float& deltaTime);

    SDL_Window* mWindow;
    SDL_Renderer* mRenderer;
    bool mIsRunning;
    bool mIsPaused;
    int mWidth;
    int mHeight;
    Vector2<int> mPaddlePosition;
    Vector2<int> mBallPosition;
    Vector2<float> mBallVelocity;
    int mThickness;
    int mPaddleHeight;
    int mPaddleDir;
    float mPaddleVelocity;
    Uint32 mTicksCount;
    int frameRate;
    int mScore;
    int mNextScore;
    TTF_Font* mFont;
};

#endif
