#pragma once

#include "Game.hpp"
#include "MovePath.hpp"

auto evaluate(Game &game) -> int;
auto printPrincipalVariation(Game game, const MovePath &movePath, int evaluation) -> void;
