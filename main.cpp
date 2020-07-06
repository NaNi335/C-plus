#include <memory>
#include <random>
#include <ctime>
#include "game/game.h"
#include "./game/game.cpp"


int main() {
    cout << "Hi! You are playing draughts. Men go first" << endl;
    play();
//    autoplay();
    return 0;
}
