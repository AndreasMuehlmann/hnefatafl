#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_set>

#include <argparse/argparse.hpp>

auto main(int argc, char* argv[]) -> int { 
    argparse::ArgumentParser program("hnefatafl", "v0.0.1");
    
    std::string attackingPlayer;
    std::string defendingPlayer;
    bool printAvailablePlayers = false;
    
    program.add_argument("attackingPlayer")
        .help("Pass who should be the attacker, so the player without the king.").store_into(attackingPlayer);
    
    program.add_argument("defendingPlayer")
        .help("Pass who should be the defender, so the player with the king.").store_into(defendingPlayer);
    
    program.add_argument("--printAvailablePlayers").store_into(printAvailablePlayers);
    
    try {
        program.parse_args(argc, argv);
    }
    catch (const std::exception& err) {
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
        for (const auto& player : availablePlayers) {
            std::cout << player << '\n';
        }
        return 1;
    }

    if (!availablePlayers.contains(attackingPlayer)) {
        throw std::invalid_argument(attackingPlayer + " is not a known player.");
    }
    if (!availablePlayers.contains(defendingPlayer)) {
        throw std::invalid_argument(defendingPlayer + " is not a known player.");
    }

     
    std::cout << attackingPlayer << " " << defendingPlayer << '\n';
}
