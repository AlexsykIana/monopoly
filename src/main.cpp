#include "Game.h"
#include <cstdlib>
#include <ctime>

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));

    Game monopolyGame;
    monopolyGame.run();

    return 0;
}