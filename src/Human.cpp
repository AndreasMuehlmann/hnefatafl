#include <iostream>
#include <sstream>

#include "Game.hpp"
#include "GameUtils.hpp"
#include "Human.hpp"

auto Human::getMove(const Game &game) -> Move {
    game.printField();

    uint8_t xFrom = 0;
    uint8_t yFrom = 0;
    uint8_t xTo = 0;
    uint8_t yTo = 0;
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
