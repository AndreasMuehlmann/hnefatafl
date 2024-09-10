#include <cstdint>
#include <iostream>
#include <stdexcept>

#include "Game.hpp"
#include "GameUtils.hpp"
#include "Move.hpp"

auto positionToCoordinates(const Position &position) -> Coordinates {
    uint8_t x = position % static_cast<uint8_t>(FIELD_SIZE);
    auto y = static_cast<uint8_t>(position / FIELD_SIZE);
    return {x, y};
}

auto coordinatesToPosition(const Coordinates &coordinates) -> Position {
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
    constexpr InternalField mask(3);
    return mask << position * BITS_PER_FIELD;
}

auto printMove(const Move &m) -> void {
    const auto from = positionToCoordinates(m.from);
    const auto to = positionToCoordinates(m.to);
    std::cout << from.x << ", " << from.y << "; " << to.x << ", " << to.y;
    std::cout << " / " << static_cast<unsigned int>(m.from) << "; "
              << static_cast<unsigned int>(m.to) << '\n';
}
