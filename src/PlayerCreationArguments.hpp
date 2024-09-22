#pragma once

#include <string>

struct PlayerCreationArguments {
    std::string attacker;
    std::string defender;
    unsigned int timeToThink;
};
