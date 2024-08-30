#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "Engine.hpp"
#include "Game.hpp"

auto main() -> int {
    Field field = {
        std::array<Figur, FIELD_SIZE>{Figur::None, Figur::None, Figur::Wiking, Figur::None, Figur::King, Figur::None, Figur::Guard, Figur::None, Figur::None},
        std::array<Figur, FIELD_SIZE>{Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
        std::array<Figur, FIELD_SIZE>{Figur::None, Figur::None, Figur::None, Figur::Wiking, Figur::Guard, Figur::None, Figur::Wiking, Figur::None, Figur::None},
        std::array<Figur, FIELD_SIZE>{Figur::None, Figur::Wiking, Figur::Wiking, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::Wiking},
        std::array<Figur, FIELD_SIZE>{Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::Wiking},
        std::array<Figur, FIELD_SIZE>{Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::Wiking},
        std::array<Figur, FIELD_SIZE>{Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
        std::array<Figur, FIELD_SIZE>{Figur::None, Figur::None, Figur::None, Figur::None, Figur::Wiking, Figur::Guard, Figur::None, Figur::None, Figur::Wiking},
        std::array<Figur, FIELD_SIZE>{Figur::None, Figur::None, Figur::Wiking, Figur::None, Figur::None, Figur::Wiking, Figur::None, Figur::None, Figur::None},
    };

    Game game(field);

    Engine engine(game, 4);

    std::string input;

    while (true) {
        game.printField();

        Move move{};
        if (game.areWikingsToMove()) {
            move = engine.getMove();
        } else {
            int xFrom = 0;
            int yFrom = 0;
            int xTo = 0;
            int yTo = 0;
            char comma = 0;
            char semicolon = 0;

            while (true) {
                std::cout << "Enter coordinates (format: x1, y1; x2, y2): ";
                std::getline(std::cin, input);
                std::stringstream stringStream(input);
                if (stringStream >> xFrom >> comma >> yFrom >> semicolon >> xTo >> comma >> yTo) {
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
            } 
            if (winner == Figur::Wiking) {
                game.printField();
                std::cout << "The wikings won!!" << std::endl;
                break;
            }
        } catch (std::invalid_argument &e) {
            std::cout << e.what() << std::endl;
        }
    }
}
