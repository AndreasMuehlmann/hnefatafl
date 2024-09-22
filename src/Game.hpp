#pragma once

#include <array>
#include <bitset>
#include <vector>

#include "Figur.hpp"
#include "Move.hpp"
#include "Winner.hpp"

constexpr std::size_t FIELD_SIZE = 9;
constexpr std::size_t FIELDS = FIELD_SIZE * FIELD_SIZE;
constexpr std::size_t BITS_PER_FIELD = 2;
constexpr std::size_t BITS_FOR_KING_POSITION = 8;
constexpr std::size_t BIT_FOR_WHOS_TO_MOVE = 1;

using Field = std::array<std::array<Figur, FIELD_SIZE>, FIELD_SIZE>;

using InternalField =
    std::bitset<FIELDS * BITS_PER_FIELD + BITS_FOR_KING_POSITION + BIT_FOR_WHOS_TO_MOVE>;

class Game {

  public:
    Game();
    Game(Field field, bool wikingsToMove);
    Game(InternalField internalField);
    [[nodiscard]] auto getFigurAt(Position position) const -> Figur;
    [[nodiscard]] auto getKingPosition() const -> Position;
    [[nodiscard]] auto areAttackersToMove() const -> bool;
    [[nodiscard]] auto getInternalField() const -> InternalField;
    [[nodiscard]] auto validMove(Move m) const -> std::string;
    auto makeMove(Move m) -> Winner;
    auto unmakeMove() -> void;
    auto printField() const -> void;

  private:
    auto construct() -> void;
    auto move(Move m) -> void;
    [[nodiscard]] auto updateField(Position lastMovedTo) -> bool;
    [[nodiscard]] auto capture(Position lastMovedTo, int shift) -> bool;
    [[nodiscard]] auto whoWon(Position lastMovedTo) const -> Winner;
    [[nodiscard]] auto draw() const -> bool;
    auto setKingPosition(Position position) -> void;

    InternalField m_field;
    std::vector<InternalField> m_history;
};
