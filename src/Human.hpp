#include "Player.hpp"

class Human : Player {
    auto getMove(const Game &game) -> Move override;
};
