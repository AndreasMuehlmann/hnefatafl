#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_set>

#include <argparse/argparse.hpp>

#include "Human.hpp"
#include "RandomMovesGenerator.hpp"
#include "Perft.hpp"
#include "SingleGameManager.hpp"

auto main(int argc, char *argv[]) -> int {
    argparse::ArgumentParser program("hnefatafl", "v0.0.1");

    std::string attacker;
    std::string defender;
    bool printAvailablePlayers = false;
    unsigned int perftDepth = 0;

    program.add_argument("--attacker")
        .store_into(attacker)
        .help("Pass who should be the attacker, so the player without the king.");

    program.add_argument("--defender")
        .store_into(defender)
        .help("Pass who should be the defender, so the player with the king.");

    program.add_argument("--printAvailablePlayers")
        .store_into(printAvailablePlayers)
        .help("Prints all player types that can be used.");

    program.add_argument("--perft")
        .scan<'i', unsigned int>()
        .store_into(perftDepth)
        .help("Run perft with a depth and print the output.");

    try {
        program.parse_args(argc, argv);
    } catch (const std::exception &err) {
        std::cerr << err.what() << std::endl;
        std::cerr << program;
        return 1;
    }

    std::unordered_set<std::string> availablePlayers = {
        "human",
        "computer",
    };

    if (printAvailablePlayers) {
        std::cout << "Available players: \n";
        for (const auto &player : availablePlayers) {
            std::cout << player << '\n';
        }
        return 1;
    }

    if (perftDepth > 0) {
        Perft perft(perftDepth);
        Game game;
        std::cout << "Running perft...\n";
        perft.perft(game);
        return 1;
    }

    if (attacker.empty() || defender.empty()) {
        throw std::invalid_argument("--attacker and --defender have to be passed");
    }
    if (!availablePlayers.contains(attacker)) {
        throw std::invalid_argument(attacker + " is not a known player.");
    }
    if (!availablePlayers.contains(defender)) {
        throw std::invalid_argument(defender + " is not a known player.");
    }

    Game game;
    std::unique_ptr<Player> attackingPlayer = std::make_unique<Human>();
    std::unique_ptr<Player> defendingPlayer = std::make_unique<RandomMovesGenerator>();
    SingleGameManager singleGameManager(game, std::move(attackingPlayer),
                                        std::move(defendingPlayer));
    singleGameManager.run();
}
