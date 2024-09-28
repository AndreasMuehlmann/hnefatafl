#pragma once

#include "MovePath.hpp"
#include "Game.hpp"

auto evaluate(Game &game) -> int;
auto printPrincipalVariation(Game game, const MovePath &movePath, int evaluation) -> void;
