#pragma once

#include <array>
#include <bitset>

#include "Faction.hpp"
#include "Move.hpp"

constexpr std::size_t FIELD_SIZE = 9;

enum Figur {
    NoFigur,
    Wiking,
    Guard,
    King,
};

using Field = std::array<std::array<Figur, FIELD_SIZE>, FIELD_SIZE>;

constexpr std::size_t FIELDS = FIELD_SIZE * FIELD_SIZE;
constexpr std::size_t BITS_PER_FIELD = 2;
constexpr std::size_t BITS_FOR_KING_POSITION = 8;
using InternalField = std::bitset<FIELDS * BITS_PER_FIELD + BITS_FOR_KING_POSITION>;

class Game {

  public:
    Game();
    Game(Field field, bool wikingsToMove);
    [[nodiscard]] auto getFigurAt(Position position) const -> Figur;
    auto makeMove(const Move &move) -> Faction;
    auto unmakeMove() -> Faction;
    auto validMove(const Move &move) -> std::string;
    auto printField() const -> void;

  private:
    auto move(const Move &move) -> void;
    auto updateField(const Position &lastMovedTo) -> void;
    [[nodiscard]] auto whoWon() const -> Faction;

    InternalField m_field;
    bool m_wikingsToMove;
};
