#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "Game.hpp"


int main() {
    Game game; 

    unsigned int xFrom, yFrom, xTo, yTo;
    char comma, semicolon;

    std::string input;

    while (true) {
        game.printField();
        std::cout << "Enter coordinates (format: x1, y1; x2, y2): ";
        std::getline(std::cin, input);

        std::stringstream ss(input);
        if (ss >> xFrom >> comma >> yFrom >> semicolon >> xTo >> comma >> yTo) {
            try {
                struct Position from {xFrom, yFrom};
                struct Position to {xTo, yTo};
                game.move(from, to);
                game.updateField(to);
                if (game.isGameOver(to)) {
                    if (game.areWikingsToMove()) {
                        std::cout << "The wikings won!!" << std::endl;
                    } else {
                        std::cout << "The king won!!" << std::endl;
                    }
                    break;
                };
                game.moveDone();
            } catch (std::invalid_argument &e) {
                std::cout << e.what() << std::endl;
            }
        } else {
            std::cerr << "Invalid input format." << std::endl;
        }
    }
}
