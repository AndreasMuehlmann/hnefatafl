#pragma once

#include "EvaluatedMovePath.hpp"
#include "Game.hpp"

auto evaluate(Game &game) -> int;
auto printPrincipalVariation(Game game, const EvaluatedMovePath& evaluatedMovePath) -> void;
