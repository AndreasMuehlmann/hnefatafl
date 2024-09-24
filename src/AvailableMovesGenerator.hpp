#pragma once

#include <array>
#include <optional>

#include "Figur.hpp"
#include "Game.hpp"
#include "Move.hpp"

constexpr std::size_t DIRECTIONS = 4;
constexpr std::array<int, DIRECTIONS> POSITION_DELTAS = {1, -1, 9, -9};

class AvailableMovesGenerator {

  public:
    AvailableMovesGenerator(const Game &game);
    auto next() -> std::optional<Move>;

  private:
    auto nextFigur() -> bool;

    const Game &m_game;
    Position m_currentFigurPosition;
    std::size_t m_positionDeltaIndex;
    int m_currentTargetPositionForMove;
    bool (*m_isToMove)(Figur);
};
