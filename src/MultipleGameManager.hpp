#pragma once

#include "PlayerFactory.hpp"

constexpr size_t TABLE_WIDTH = 20;

class MultipleGameManager {

  public:
    MultipleGameManager(PlayerFactory playerFactory, size_t games, size_t randomMoveDepthForStartState);
    auto run() const -> void;

  private:
    PlayerFactory m_playerFactory;
    size_t m_games;
    size_t m_randomMoveDepthForStartState;
};
