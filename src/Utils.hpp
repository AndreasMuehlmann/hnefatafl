#include <memory>
#include <string>

class Player;

auto createPlayerFromIdentifier(const std::string& identifier) -> std::unique_ptr<Player>;
auto rightPad(std::string string, size_t paddedLength) -> std::string;
