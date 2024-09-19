#include <cstdint>
#include <cstdlib>
#include <iostream>

#include "AvailableMovesGenerator.hpp"
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
    construct();
}

Game::Game(Field field, bool wikingsToMove) : m_field(fieldToInternalField(field)) {
    if (wikingsToMove) {
        m_field.set(INDEX_WIKINGS_TO_MOVE_FLAG);
    } else {
        m_field.reset(INDEX_WIKINGS_TO_MOVE_FLAG);
    }
    construct();
}

Game::Game(InternalField internalField) : m_field(internalField) { construct(); }

auto Game::construct() -> void {
    constexpr size_t RESERVED_SIZE_HISTORY = 100;
    m_history.reserve(RESERVED_SIZE_HISTORY);
}

auto Game::getFigurAt(Position position) const -> Figur {
    return static_cast<Figur>((static_cast<uint8_t>(m_field._Unchecked_test(position * BITS_PER_FIELD + 1)) << 1) + static_cast<uint8_t>(m_field._Unchecked_test(position * BITS_PER_FIELD)));
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

auto Game::areAttackersToMove() const -> bool { return m_field._Unchecked_test(INDEX_WIKINGS_TO_MOVE_FLAG); }

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

    AvailableMovesGenerator availableMovesGenerator(*this);
    while (true) {
        auto moveOption = availableMovesGenerator.next();
        if (moveOption == std::nullopt) { break; }
        if (m.from == moveOption->from && m.to == moveOption->to) {
            return "";
        }
    }

    return "Cannot move because figur is in the way.";
}

auto Game::makeMove(Move m) -> Winner {
    m_history.push_back(m_field);
    move(m);
    if (updateField(m.to)) {
        return Winner::Attacker;
    };
    Winner winner = whoWon(m.to);
    if (winner == Winner::NoWinner && draw()) {
        winner = Winner::Draw;
    }
    m_field._Unchecked_flip(INDEX_WIKINGS_TO_MOVE_FLAG);
    return winner;
}

auto Game::move(Move m) -> void {
    const auto mask = maskForPosition(m.from);
    const auto fieldWithOnlyFigurMoved = m_field & mask;
    if (getFigurAt(m.from) == Figur::King) {
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
                if (figur == Figur::King) { 
                    return maskedFieldMatchesPosition(m_field, MASK_KING_SURROUNDED_IN_CASTLE, MASK_AROUND_CASTLE);
                }
                m_field &= ~mask;
                return capturingKing;
            }
            if (figur == Figur::NoFigur) { return false; }
            if (figur == Figur::Wiking) {
                m_field &= ~mask;
                return capturingKing;
            };
            if (figur == Figur::King) { 
                if (position == FIELDS / 2 + 1) {
                    return maskedFieldMatchesPosition(m_field, MASK_KING_SURROUNDED_RIGHT_CASTLE, MASK_RIGHT_CASTLE);
                }
                if (position == FIELDS / 2 - 1) {
                    return maskedFieldMatchesPosition(m_field, MASK_KING_SURROUNDED_LEFT_CASTLE, MASK_LEFT_CASTLE);
                }
                if (position == FIELDS / 2 + FIELD_SIZE) {
                    return maskedFieldMatchesPosition(m_field, MASK_KING_SURROUNDED_BELOW_CASTLE, MASK_BELOW_CASTLE);
                }
                if (position == FIELDS / 2 - FIELD_SIZE) {
                    return maskedFieldMatchesPosition(m_field, MASK_KING_SURROUNDED_ABOVE_CASTLE, MASK_ABOVE_CASTLE);
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

auto Game::whoWon(Position lastMovedTo) const -> Winner {
    if (getFigurAt(lastMovedTo) != Figur::King) {
        return Winner::NoWinner;
    }
    Coordinates kingCoordinates = positionToCoordinates(lastMovedTo);
    if (kingCoordinates.x == 0 || kingCoordinates.x == FIELD_SIZE - 1 || kingCoordinates.y == 0 ||
        kingCoordinates.y == FIELD_SIZE - 1) {
        return Winner::Defender;
    }
    return Winner::NoWinner;
}

auto Game::draw() const -> bool {
    // check if current position occured 3 times, ignore who is to move
    // check if there is at least one move available

    return false;
}

auto Game::unmakeMove() -> void {
    m_field._Unchecked_flip(INDEX_WIKINGS_TO_MOVE_FLAG);
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
