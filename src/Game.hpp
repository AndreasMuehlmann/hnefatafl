#pragma once

#include <array>

constexpr std::size_t FIELD_SIZE = 9;

enum Figur { None, King, Wiking, Guard };

struct Vec2D {
    int x;
    int y;
};

struct Move {
    Vec2D from;
    Vec2D to;
};

using Field = std::array<std::array<Figur, FIELD_SIZE>, FIELD_SIZE>;

class Game {

  public:
    Game();
    Game(Field field, bool wikingsToMove);
    [[nodiscard]] auto areWikingsToMove() const -> bool;
    [[nodiscard]] auto getFigurAt(Vec2D position) const -> Figur;
    [[nodiscard]] auto getField() const -> const Field &;
    [[nodiscard]] auto getWikingCount() const -> unsigned int;
    [[nodiscard]] auto getGuardCount() const -> unsigned int;
    [[nodiscard]] static auto isPositionInBounds(Vec2D position) -> bool;
    auto moveUnchecked(Move move) -> void;
    auto move(Move move) -> void;
    auto updateField(Vec2D lastMovedTo) -> void;
    [[nodiscard]] auto whoWon() const -> Figur;
    auto printField() const -> void;

  private:
    Field m_field;
    bool m_wikingsToMove;
    Vec2D m_kingPosition;
    unsigned int m_guardCount;
    unsigned int m_wikingCount;

    void capture(Vec2D lastMovedTo, Vec2D direction);
    [[nodiscard]] auto isBlocked(Move move) const -> bool;
    void setFigurAt(Figur figur, Vec2D position);
};
