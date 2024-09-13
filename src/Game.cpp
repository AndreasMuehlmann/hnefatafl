#include <cstdint>
#include <cstdlib>
#include <iostream>

#include "BitMasks.hpp"
#include "FieldDefinitionHelper.hpp"
#include "Game.hpp"
#include "GameUtils.hpp"
#include "Move.hpp"

constexpr size_t INDEX_WIKINGS_TO_MOVE_FLAG = FIELDS * BITS_PER_FIELD + BITS_FOR_KING_POSITION;

Game::Game() : m_availableMovesGenerator(*this) {
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
    construct();
}

Game::Game(Field field, bool wikingsToMove) : m_field(fieldToInternalField(field)), m_availableMovesGenerator(*this) {
    if (wikingsToMove) {
        m_field.set(INDEX_WIKINGS_TO_MOVE_FLAG);
    } else {
        m_field.reset(INDEX_WIKINGS_TO_MOVE_FLAG);
    }
    construct();
}

Game::Game(InternalField internalField) : m_field(internalField), m_availableMovesGenerator(*this) { construct(); }

auto Game::construct() -> void {
    constexpr size_t RESERVED_SIZE_HISTORY = 100;
    m_history.reserve(RESERVED_SIZE_HISTORY);

    m_positionCounts[m_field] = 1;
}

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

auto Game::getAvailableMovesGenerator() -> AvailableMovesGenerator& {
    m_availableMovesGenerator.reset();
    return m_availableMovesGenerator;
}

auto Game::validMove(Move m) const -> std::string {
    if (!positionInBounds(m.from)) {
        return "Position to move away from out of field range.";
    }
    if (!positionInBounds(m.to)) {
        return "Position to move to out of field range.";
    }
    if (m.from == m.to) {
        return "Position to move to and position to move from are equal.";
    }

    Figur figur = getFigurAt(m.from);
    if (figur == Figur::NoFigur) {
        return "On the position to move away from is no figur.";
    }
    if ((areAttackersToMove() && isDefender(getFigurAt(m.from))) ||
        (!areAttackersToMove() && isAttacker(getFigurAt(m.from)))) {
        return "The figur belongs to the other player.";
    }
    Coordinates fromCoordinates = positionToCoordinates(m.from);
    Coordinates toCoordinates = positionToCoordinates(m.to);
    if (fromCoordinates.x != toCoordinates.x && fromCoordinates.y != toCoordinates.y) {
        return "Diagonal movement is not allowed.";
    }
    if (m.to == FIELDS / 2 && figur != Figur::King) {
        return "Cannot move into the center position "
               "unless the figur is the king.";
    }

    // check if move is blocked

    return "";
}

auto Game::makeMove(Move m) -> Winner {
    m_history.push_back(m_field);
    move(m);
    if (updateField(m.to)) {
        return Winner::Attacker;
    };
    if (m_positionCounts.contains(m_field)) {
        m_positionCounts[m_field] += 1;
    } else {
        m_positionCounts[m_field] = 1;
    }
    Winner winner = whoWon();
    if (winner == Winner::NoWinner && draw()) {
        winner = Winner::Draw;
    }
    m_field.flip(INDEX_WIKINGS_TO_MOVE_FLAG);
    return winner;
}

auto Game::move(Move m) -> void {
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

auto Game::updateField(Position lastMovedTo) -> bool {
    if (!possibleCapture(m_field, lastMovedTo, areAttackersToMove())) {
        return false;
    }
    if (capture(lastMovedTo, 1)) { return true; }
    if (capture(lastMovedTo, -1)) { return true; }
    if (capture(lastMovedTo, static_cast<int>(FIELD_SIZE))) { return true; }
    if (capture(lastMovedTo, -static_cast<int>(FIELD_SIZE))) { return true; }
    return false;
}

auto Game::capture(Position lastMovedTo, int shift) -> bool {
    auto bitShift = bitShiftLeft;
    if (shift < 0) {
        bitShift = bitShiftRight;
    }
    auto absShift = static_cast<size_t>(abs(shift));
    InternalField mask;
    if (areAttackersToMove()) {
        bool capturingKing = false;
        for (int position = lastMovedTo + shift; 0 < position && position < static_cast<int>(FIELDS); position += shift) {
            Figur figur = getFigurAt(static_cast<Position>(position));
            if (position == FIELDS / 2) {
                if (figur == Figur::King) { return false; }
                m_field &= ~mask;
                return capturingKing;
            }
            if (figur == Figur::NoFigur) { return false; }
            if (figur == Figur::Wiking) {
                m_field &= ~mask;
                return capturingKing;
            };
            if (figur == Figur::King) { 
                if (position == FIELDS / 2 + 1
                    || position == FIELDS / 2 - 1
                    || position == FIELDS / 2 + FIELD_SIZE
                    || position == FIELDS / 2 - FIELD_SIZE) {
                    return false;
                }
                capturingKing = true;
            }
            mask |= maskForPosition(lastMovedTo);
            mask = bitShift(mask, static_cast<uint8_t>(absShift * BITS_PER_FIELD)) ;
        }
    } else {
        for (int position = lastMovedTo + shift; 0 < position && position < static_cast<int>(FIELDS); position += shift) {
            Figur figur = getFigurAt(static_cast<Position>(position));
            if (position == FIELDS / 2) {
                m_field &= ~mask;
                return false;
            }
            if (figur == Figur::NoFigur) { return false; }
            if (figur == Figur::Guard || figur == Figur::King) {
                m_field &= ~mask;
                return false;
            };
            mask |= maskForPosition(lastMovedTo);
            mask = bitShift(mask, static_cast<uint8_t>(absShift * BITS_PER_FIELD)) ;
        }

    }
    return false;
}

auto Game::whoWon() const -> Winner {
    Coordinates kingCoordinates = positionToCoordinates(getKingPosition());
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
    if (m_positionCounts.at(m_field) >= 3) {
        return true;
    }

    // check if there is at least one move available

    return false;
}

auto Game::unmakeMove() -> void {
    uint8_t &positionCount = m_positionCounts.at(m_field);
    positionCount -= 1;
    if (positionCount == 0) {
        m_positionCounts.erase(m_field);
    }
    m_field = m_history.at(m_history.size() - 1);
    m_history.pop_back();
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
