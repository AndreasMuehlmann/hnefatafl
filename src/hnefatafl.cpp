#include "Game.hpp"


int main() {
    Game game; 
    struct Position from = { 4, 1 };
    struct Position to = { 5, 1 };
    game.move(from, to);
    game.printField();
}
