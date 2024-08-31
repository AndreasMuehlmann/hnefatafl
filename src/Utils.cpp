#include <iostream>

#include "Engine.hpp"
#include "Game.hpp"

auto sign(int x) -> int { return static_cast<int>(x > 0) - static_cast<int>(x < 0); }

auto isKingAttacker(Figur figur) -> bool { return figur == Figur::Wiking; }

auto isKingDefender(Figur figur) -> bool { return figur == Figur::Guard || figur == Figur::King; }

auto moveAlongPath(Game game, const EvaluatedMovePath &evaluatedMovePath) -> void {
    std::cout << std::endl << std::endl;
    std::cout << "HOT PATH" << std::endl;
    std::cout << "Evaluation: " << evaluatedMovePath.evaluation << std::endl;
    std::cout << "Size: " << evaluatedMovePath.movePath.size() << std::endl;
    game.printField();
    for (size_t i = 0; i < evaluatedMovePath.movePath.size(); i++) {
        MoveWithId move = evaluatedMovePath.movePath[evaluatedMovePath.movePath.size() - i - 1];
        std::cout << "Move: " << move.move.from.x << ", " << move.move.from.y << "; "
                  << move.move.to.x << ", " << move.move.to.y << " | id " << move.id << " depth "
                  << move.depth << std::endl;
        game.move(move.move);
        game.updateField(move.move.to);
        game.printField();
        if (game.whoWon() == Figur::King) {
            std::cout << "King won" << std::endl;
        } else if (game.whoWon() == Figur::Wiking) {
            std::cout << "Wiking won" << std::endl;
        }
    }
    std::cout << std::endl << std::endl;
}
