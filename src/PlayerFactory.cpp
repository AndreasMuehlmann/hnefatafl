#include <memory>

#include "Human.hpp"
#include "Negamax.hpp"
#include "PlayerFactory.hpp"
#include "RandomMovesGenerator.hpp"

constexpr unsigned int oneSecond = 1000;

PlayerFactory::PlayerFactory(PlayerCreationArguments playerCreationArguments)
    : m_playerCreationArguments(std::move(playerCreationArguments)) {}

auto PlayerFactory::createAttacker() const -> std::unique_ptr<Player> {
    return create(m_playerCreationArguments.attacker);
}

auto PlayerFactory::createDefender() const -> std::unique_ptr<Player> {
    return create(m_playerCreationArguments.defender);
}
auto PlayerFactory::create(const std::string &identifier) const -> std::unique_ptr<Player> {
    if (identifier == "human") {
        return std::make_unique<Human>();
    }
    if (identifier == "random") {
        return std::make_unique<RandomMovesGenerator>();
    }
    if (identifier == "negamax") {
        if (m_playerCreationArguments.timeToThink != 0) {
            return std::make_unique<Negamax>(m_playerCreationArguments.timeToThink);
        }
        return std::make_unique<Negamax>(oneSecond);
    }
    throw std::invalid_argument("Given identifier is in availablePlayers but not handled.");
}
