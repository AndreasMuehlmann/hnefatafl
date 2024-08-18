#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>


#include "Game.hpp"
#include "Engine.hpp"


int main() {
    Game game;

    Engine engine(game, 6);

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
            Figur winner = game.whoWon();
            if (winner == Figur::King) {
                game.printField();
                std::cout << "The king won!!" << std::endl;
                break;
            } else if (winner == Figur::Wiking) {
                game.printField();
                std::cout << "The wikings won!!" << std::endl;
                break;
            }
        } catch (std::invalid_argument &e) {
            std::cout << e.what() << std::endl;
        }
    }
}
