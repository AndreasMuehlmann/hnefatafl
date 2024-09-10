#pragma once

#include <cstdint>

using Position = uint8_t;

struct Coordinates {
    unsigned int x;
    unsigned int y;
};

struct Move {
    Position from;
    Position to;
};
