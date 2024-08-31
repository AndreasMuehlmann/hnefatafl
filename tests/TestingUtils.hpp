#pragma once

#include "Game.hpp"

auto createGameFromField(Field &field, bool areWikingsToMove) -> Game;
auto transform(Field &field) -> void;
