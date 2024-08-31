#pragma once

#include "Game.hpp"

auto sign(int x) -> int;

auto isKingAttacker(Figur figur) -> bool;

auto isKingDefender(Figur figur) -> bool;

struct EvaluatedMovePath;

auto moveAlongPath(Game game, const EvaluatedMovePath &evaluatedMovePath) -> void;
