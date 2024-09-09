#include <cstdint>

#include "GameUtils.hpp"
#include "Game.hpp"

auto positionToCoordinates(Position position) -> Coordinates {
    uint8_t x = position % FIELD_SIZE;
    auto y = static_cast<uint8_t>(position / FIELD_SIZE);
    return {x, y};
}

auto coordinatesToPosition(Coordinates coordinates) -> Position {
    return coordinates.x + coordinates.y * FIELD_SIZE;
}
