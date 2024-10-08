#include <cstdint>
#include <iostream>
#include <stdexcept>

#include "Game.hpp"
#include "GameUtils.hpp"
#include "Move.hpp"

auto positionToCoordinates(Position position) -> Coordinates {
    uint8_t x = position % static_cast<uint8_t>(FIELD_SIZE);
    auto y = static_cast<uint8_t>(position / FIELD_SIZE);
    return {x, y};
}

auto coordinatesToPosition(Coordinates coordinates) -> Position {
    return static_cast<uint8_t>(coordinates.x) + static_cast<uint8_t>(coordinates.y * FIELD_SIZE);
}

auto fieldToInternalField(const Field &field) -> InternalField {
    InternalField internalField;
    unsigned int kingCount = 0;
    for (uint8_t y = 0; y < static_cast<uint8_t>(FIELD_SIZE); y++) {
        for (uint8_t x = 0; x < static_cast<uint8_t>(FIELD_SIZE); x++) {
            Figur figur = field.at(y).at(x);
            if (figur == Figur::King) {
                kingCount++;
                if (kingCount > 1) {
                    throw std::invalid_argument("Multiple kings are not allowed.");
                }
                auto kingPosition = static_cast<InternalField>(coordinatesToPosition({x, y}));
                internalField |= kingPosition << FIELD_SIZE * FIELD_SIZE * BITS_PER_FIELD;
            }
            const auto figurBits = static_cast<InternalField>(figur);
            internalField |= figurBits << coordinatesToPosition({x, y}) * BITS_PER_FIELD;
        }
    }
    return internalField;
}

auto maskForPosition(Position position) -> InternalField {
    InternalField mask;
    mask._Unchecked_set(position * BITS_PER_FIELD);
    mask._Unchecked_set(position * BITS_PER_FIELD + 1);
    return mask;
}

auto numberToPosition(Position position, uint8_t number) -> InternalField {
    InternalField mask(number);
    return mask << position * BITS_PER_FIELD;
}

auto maskedFieldMatchesPosition(const InternalField &field, const InternalField &mask,
                                const InternalField &position) -> bool {
    return (field & position) == mask;
}

auto printCoordinates(Coordinates coordinates) -> void {
    std::cout << coordinates.x << ", " << coordinates.y << '\n';
}

auto printMove(Move m) -> void {
    const auto from = positionToCoordinates(m.from);
    const auto to = positionToCoordinates(m.to);
    std::cout << from.x << ", " << from.y << "; " << to.x << ", " << to.y;
    std::cout << " / " << static_cast<unsigned int>(m.from) << "; "
              << static_cast<unsigned int>(m.to) << '\n';
}

auto positionInBounds(Position position) -> bool { return position < FIELDS; }

auto isDefender(Figur figur) -> bool { return figur == Figur::Guard || figur == Figur::King; }

auto isAttacker(Figur figur) -> bool { return figur == Figur::Wiking; }

auto bitShiftLeft(const InternalField &field, uint8_t shift) -> InternalField {
    return field << shift;
}

auto bitShiftRight(const InternalField &field, uint8_t shift) -> InternalField {
    return field >> shift;
}

auto possibleCapture(const InternalField &field, Position lastMovedTo,
                     bool attackersToMove) -> bool {
    const auto coordinates = positionToCoordinates(lastMovedTo);
    Position offset = 0;
    if (attackersToMove) {
        offset = 1;
    }
    bool possibleCatpure = false;
    if (coordinates.x != FIELD_SIZE - 1) {
        possibleCatpure |= field._Unchecked_test((lastMovedTo + 1) * BITS_PER_FIELD + offset);
    }
    if (coordinates.x != 0) {
        possibleCatpure |= field._Unchecked_test((lastMovedTo - 1) * BITS_PER_FIELD + offset);
    }
    if (coordinates.y != FIELD_SIZE - 1) {
        possibleCatpure |=
            field._Unchecked_test((lastMovedTo + FIELD_SIZE) * BITS_PER_FIELD + offset);
    }
    if (coordinates.y != 0) {
        possibleCatpure |=
            field._Unchecked_test((lastMovedTo - FIELD_SIZE) * BITS_PER_FIELD + offset);
    }
    return possibleCatpure;
}

auto isDifferentRow(Position position1, Position position2) -> bool {
    return position1 / static_cast<Position>(FIELD_SIZE) !=
           position2 / static_cast<Position>(FIELD_SIZE);
}
