#pragma once

#include "Move.hpp"

constexpr std::size_t MAX_MOVE_COUNT = 10;

struct MovePath {
    std::size_t moveCount;
    Move moves[MAX_MOVE_COUNT];
};
