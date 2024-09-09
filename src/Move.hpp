#pragma once

#include <cstdint>

using Position = uint8_t;

struct Coordinates {
    uint8_t x;
    uint8_t y;
};

struct Move {
    Position from;
    Position to;
};
