#include <iostream>

#include "Game.hpp"
#include "FieldDefinitionHelper.hpp"
#include "GameUtils.hpp"

Game::Game() : m_wikingsToMove(true) {
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

Game::Game(Field field, bool wikingsToMove) : m_field(fieldToInternalField(field)), m_wikingsToMove(wikingsToMove) {}

auto Game::getFigurAt(Position position) const -> Figur {
    constexpr InternalField mask(3);
    InternalField movedMask = (mask << position * BITS_PER_FIELD);
    auto figur = static_cast<Figur>(((m_field & movedMask) >> position * BITS_PER_FIELD).to_ulong());
    return figur;
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
