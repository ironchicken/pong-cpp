#ifndef GAME_H
#define GAME_H

#include "SDL2/SDL.h"

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
};

#endif
