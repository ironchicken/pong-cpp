#ifndef GAME_H
#define GAME_H

#include "SDL2/SDL.h"

struct Vector2 {
    int x;
    int y;
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
    int mWidth;
    int mHeight;
    Vector2 mPaddlePosition;
    Vector2 mBallPosition;
    int mThickness;
    int mPaddleHeight;
    int mPaddleDir;
    float mPaddleVelocity;
    Uint32 mTicksCount;
    int frameRate;
};

#endif
