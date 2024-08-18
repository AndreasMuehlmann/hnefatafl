#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "Game.hpp"
#include "Engine.hpp"


int main() {
    Game game;

    Engine engine(game, 5);

    std::string input;

    while (true) {
        game.printField();

        Move move;
        if (game.areWikingsToMove()) {
            move = engine.getMove();
        } else {
            int xFrom, yFrom, xTo, yTo;
            char comma, semicolon;

            while (true) {
                std::cout << "Enter coordinates (format: x1, y1; x2, y2): ";
                std::getline(std::cin, input);
                std::stringstream ss(input);
                if (ss >> xFrom >> comma >> yFrom >> semicolon >> xTo >> comma >> yTo) {
                    break;
                }
            }
            std::cout << xFrom << " " << yFrom << std::endl;
            move = {{xFrom, yFrom}, {xTo, yTo}};
        }
        try {
            game.move(move);
            game.updateField(move.to);
            std::cout << game.isGameOver(move.to) << std::endl;
            if (game.isGameOver(move.to)) {
                if (game.areWikingsToMove()) {
                    game.printField();
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
    }
    /*
    while (true) {
        game.printField();
        std::cout << "Enter coordinates (format: x1, y1; x2, y2): ";
        std::getline(std::cin, input);

        std::stringstream ss(input);
        if (ss >> xFrom >> comma >> yFrom >> semicolon >> xTo >> comma >> yTo) {
            try {
                struct Vec2D from {xFrom, yFrom};
                struct Vec2D to {xTo, yTo};
                game.move({from, to});
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
    */
}
