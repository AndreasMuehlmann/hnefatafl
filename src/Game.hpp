#pragma once

#include "Faction.hpp"
#include "Move.hpp"

class Game {

public:
    auto makeMove(const Move& move) -> Faction;
    auto unmakeMove() -> Faction;

private:
    auto move(const Move& move) -> void;
    auto updateField(const Position& lastMovedTo) -> void;
    auto whoWon() -> Faction;
};
