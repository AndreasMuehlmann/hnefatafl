#include <iostream>

#include "BitMasks.hpp"
#include "FieldDefinitionHelper.hpp"
#include "Game.hpp"
#include "GameUtils.hpp"
#include "Move.hpp"

constexpr size_t INDEX_WIKINGS_TO_MOVE_FLAG = FIELDS * BITS_PER_FIELD + BITS_FOR_KING_POSITION;

Game::Game() {
    constexpr Field field = {
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
    m_field.set(INDEX_WIKINGS_TO_MOVE_FLAG);
}

Game::Game(Field field, bool wikingsToMove) : m_field(fieldToInternalField(field)) {
    if (wikingsToMove) {
        m_field.set(INDEX_WIKINGS_TO_MOVE_FLAG);
    } else {
        m_field.reset(INDEX_WIKINGS_TO_MOVE_FLAG);
    }
}

Game::Game(InternalField internalField) : m_field(internalField) {}

auto Game::getFigurAt(Position position) const -> Figur {
    return static_cast<Figur>(
        ((m_field & maskForPosition(position)) >> position * BITS_PER_FIELD).to_ulong());
}

auto Game::getKingPosition() const -> Position {
    return static_cast<Position>(
        ((m_field & MASK_KING_POSITION) >> FIELDS * BITS_PER_FIELD).to_ulong());
}

auto Game::setKingPosition(Position position) -> void {
    m_field &= ~MASK_KING_POSITION;
    InternalField kingPosition(position);
    m_field |= kingPosition << FIELDS * BITS_PER_FIELD;
}

auto Game::areAttackersToMove() const -> bool { return m_field.test(INDEX_WIKINGS_TO_MOVE_FLAG); }

auto Game::validMove(const Move &m) const -> std::string {
    (void)m;
    return "";
}

auto Game::makeMove(const Move &m) -> Winner {
    m_history.push_back(m_field);
    move(m);
    if (updateField(m.to)) {
        return Winner::Attacker;
    };
    Winner winner = whoWon();
    if (winner != Winner::NoWinner && draw()) {
        winner = Winner::Draw;
    }
    m_field.flip(INDEX_WIKINGS_TO_MOVE_FLAG);
    return winner;
}

auto Game::move(const Move &m) -> void {
    const auto mask = maskForPosition(m.from);
    const auto fieldWithOnlyFigurMoved = m_field & mask;
    if (m.from == getKingPosition()) {
        setKingPosition(m.to);
    }
    m_field &= ~mask;
    if (m.to > m.from) {
        m_field |= fieldWithOnlyFigurMoved << (m.to - m.from) * BITS_PER_FIELD;
    } else {
        m_field |= fieldWithOnlyFigurMoved >> (m.from - m.to) * BITS_PER_FIELD;
    }
}

auto Game::updateField(const Position &lastMovedTo) -> bool {
    (void)lastMovedTo;
    return false;
}

auto Game::whoWon() const -> Winner {
    Coordinates kingCoordinates = positionToCoordinates(getKingPosition());
    printCoordinates(kingCoordinates);
    if (kingCoordinates.x == 0 || kingCoordinates.x == FIELD_SIZE - 1 || kingCoordinates.y == 0 ||
        kingCoordinates.y == FIELD_SIZE - 1) {
        return Winner::Defender;
    }
    if (maskedFieldMatchesPosition(m_field, MASK_KING_SURROUNDED_IN_CASTLE, MASK_AROUND_CASTLE) ||
        maskedFieldMatchesPosition(m_field, MASK_KING_SURROUNDED_LEFT_CASTLE, MASK_LEFT_CASTLE) ||
        maskedFieldMatchesPosition(m_field, MASK_KING_SURROUNDED_RIGHT_CASTLE, MASK_RIGHT_CASTLE) ||
        maskedFieldMatchesPosition(m_field, MASK_KING_SURROUNDED_BELOW_CASTLE, MASK_BELOW_CASTLE) ||
        maskedFieldMatchesPosition(m_field, MASK_KING_SURROUNDED_ABOVE_CASTLE, MASK_ABOVE_CASTLE)) {
        return Winner::Attacker;
    }

    return Winner::NoWinner;
}

auto Game::draw() const -> bool {
    // check for repetition
    // check if there is at least one move available
    return false;
}

auto Game::printField() const -> void {
    for (Position position = 0; position < FIELD_SIZE * FIELD_SIZE; position++) {
        if (position % FIELD_SIZE == 0 && position != 0) {
            std::cout << '\n';
        }
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
