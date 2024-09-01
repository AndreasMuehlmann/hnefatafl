#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "Engine.hpp"
#include "Game.hpp"

auto main() -> int {

    constexpr Figur w = Figur::Wiking;
    constexpr Figur g = Figur::Guard;
    constexpr Figur k = Figur::King;
    constexpr Figur _ = Figur::None;
    Field field = {
        std::array<Figur, FIELD_SIZE>{_, _, w, _, k, _, g, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, w, g, _, w, _, _},
        std::array<Figur, FIELD_SIZE>{_, w, w, _, _, _, _, _, w},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, w},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, w},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, w, g, _, _, w},
        std::array<Figur, FIELD_SIZE>{_, _, w, _, _, w, _, _, _},
    };


    Game game(field, true);

    //Game game;
    constexpr unsigned int maxDepth = 7;
    Engine engine(game, maxDepth);

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
        break;
    }
}
