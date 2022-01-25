#ifndef GAME_H
#define GAME_H

#include "SDL2/SDL.h"

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
};

#endif
