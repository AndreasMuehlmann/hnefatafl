#pragma once

#include "Faction.hpp"
#include "Move.hpp"

constexpr std::size_t FIELD_SIZE = 9;

class Game {

  public:
    auto makeMove(const Move &move) -> Faction;
    auto unmakeMove() -> Faction;
    auto printField() const -> void;

  private:
    auto move(const Move &move) -> void;
    auto updateField(const Position &lastMovedTo) -> void;
    [[nodiscard]] auto whoWon() const -> Faction;
};
