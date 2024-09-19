#include <iostream>
#include <sstream>

#include "Game.hpp"
#include "GameUtils.hpp"
#include "Human.hpp"

auto Human::getMove(const Game &game) -> Move {
    game.printField();
    if (game.areAttackersToMove()) {
        std::cout << "Attackers to move\n";
    } else {
        std::cout << "Defenders to move\n";
    }

    unsigned int xFrom = 0;
    unsigned int yFrom = 0;
    unsigned int xTo = 0;
    unsigned int yTo = 0;
    char comma = 0;
    char semicolon = 0;

    std::string input;
    while (true) {
        std::cout << "Enter coordinates (format: x1, y1; x2, y2): ";
        std::getline(std::cin, input);
        std::stringstream stringStream(input);
        if (stringStream >> xFrom >> comma >> yFrom >> semicolon >> xTo >> comma >> yTo) {
            break;
        }
    }

    return {coordinatesToPosition({xFrom, yFrom}), coordinatesToPosition({xTo, yTo})};
}
