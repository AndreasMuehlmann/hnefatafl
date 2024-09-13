#pragma once

#include "Game.hpp"

class Perft {
    public:
        Perft(unsigned int maxDepth);
        auto perft(Game game) const -> void;

    private:
        static auto perftHelper(Game& game, unsigned int depth) -> unsigned long;

        unsigned int m_maxDepth;
};
