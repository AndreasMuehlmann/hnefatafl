#include <cstdint>

#include "Game.hpp"
#include "GameUtils.hpp"

auto positionToCoordinates(Position position) -> Coordinates {
    uint8_t x = position % static_cast<uint8_t>(FIELD_SIZE);
    auto y = static_cast<uint8_t>(position / FIELD_SIZE);
    return {x, y};
}

auto coordinatesToPosition(Coordinates coordinates) -> Position {
    return coordinates.x + static_cast<uint8_t>(coordinates.y * FIELD_SIZE);
}
