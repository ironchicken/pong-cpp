#include "Game.hpp"

int main(int argc, char* argv[]) {
    Game game;

    auto success = game.Initialise();
    if (success) {
        game.RunLoop();
    }

    game.ShutDown();

    return 0;
}
