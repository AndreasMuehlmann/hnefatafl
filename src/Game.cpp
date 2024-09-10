#include <iostream>

#include "Game.hpp"
#include "FieldDefinitionHelper.hpp"
#include "GameUtils.hpp"
#include "Move.hpp"

Game::Game() : m_attackersToMove(true) {
    Field field = {
        std::array<Figur, FIELD_SIZE>{_, _, _, w, w, w, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, w, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, g, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{w, _, _, _, g, _, _, _, w},
        std::array<Figur, FIELD_SIZE>{w, w, g, g, k, g, g, w, w},
        std::array<Figur, FIELD_SIZE>{w, _, _, _, g, _, _, _, w},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, g, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, w, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, w, w, w, _, _, _},
    };
    m_field = fieldToInternalField(field);
}

Game::Game(Field field, bool wikingsToMove) : m_field(fieldToInternalField(field)), m_attackersToMove(wikingsToMove) {}

auto Game::getFigurAt(Position position) const -> Figur {
    return static_cast<Figur>(((m_field & maskForPosition(position)) >> position * BITS_PER_FIELD).to_ulong());
}

auto Game::areAttackersToMove() const -> bool {
    return m_attackersToMove;
}

auto Game::makeMove(const Move &m) -> Winner {
    m_history.push_back(m_field);
    move(m);
    updateField(m.to);
    return whoWon();
}

auto Game::move(const Move &m) -> void {
    const auto mask = maskForPosition(m.from);
    const auto fieldWithOnlyFigurMoved = m_field & mask;
    m_field &= ~mask;
    if (m.to > m.from) {
        m_field |= fieldWithOnlyFigurMoved << (m.to - m.from) * BITS_PER_FIELD;
    } else {
        m_field |= fieldWithOnlyFigurMoved >> (m.from - m.to) * BITS_PER_FIELD;
    }
}

auto Game::updateField(const Position &lastMovedTo) -> void {

}

auto Game::whoWon() const -> Winner {
    return Winner::NoWinner;
}

auto Game::printField() const -> void {
    for (Position position = 0; position < FIELD_SIZE * FIELD_SIZE; position++) {
        if (position % FIELD_SIZE == 0 && position != 0) { std::cout << '\n'; }
        const auto figur = getFigurAt(position);
        switch (figur) {
            case Figur::NoFigur:
                std::cout << "_ ";
                break;
            case Figur::Wiking:
                std::cout << "w ";
                break;
            case Figur::Guard:
                std::cout << "g ";
                break;
            case Figur::King:
                std::cout << "k ";
                break;
        }
    }
    std::cout << '\n';
}
