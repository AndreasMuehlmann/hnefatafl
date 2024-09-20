#include <memory>
#include <stdexcept>

#include "Player.hpp"
#include "Human.hpp"
#include "Negamax.hpp"
#include "RandomMovesGenerator.hpp"

auto createPlayerFromIdentifier(const std::string& identifier) -> std::unique_ptr<Player> {
    if (identifier == "human") {
        return std::make_unique<Human>();
    } 
    if (identifier == "random") {
        return std::make_unique<RandomMovesGenerator>();
    }
    throw std::invalid_argument("Given identifier is in availablePlayers but not handled.");
}

auto rightPad(std::string string, size_t paddedLength) -> std::string {
  if (paddedLength < string.size()) {
    return string;
  }
  string.insert(string.end(), paddedLength - string.length(), ' ');
  return string;
}
