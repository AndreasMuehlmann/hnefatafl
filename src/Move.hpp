#pragma once

#include <cstdint>

using Position = uint8_t;

struct Move {
    Position from;
    Position to;
};
