#pragma once

#include "Player.hpp"
#include "PlayerCreationArguments.hpp"
#include <memory>

class PlayerFactory {
  public:
    PlayerFactory(PlayerCreationArguments playerCreationArguments);
    [[nodiscard]] auto createAttacker() const -> std::unique_ptr<Player>;
    [[nodiscard]] auto createDefender() const -> std::unique_ptr<Player>;
  private:
    [[nodiscard]] auto create(const std::string& identifier) const -> std::unique_ptr<Player>;

    PlayerCreationArguments m_playerCreationArguments;
};
