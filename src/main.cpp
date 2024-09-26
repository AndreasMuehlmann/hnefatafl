#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_set>

#include <argparse/argparse.hpp>

#include "MultipleGameManager.hpp"
#include "Perft.hpp"
#include "PlayerCreationArguments.hpp"
#include "PlayerFactory.hpp"
#include "SingleGameManager.hpp"
#include "GameManagerUtils.hpp"

auto main(int argc, char *argv[]) -> int {
    argparse::ArgumentParser program("hnefatafl", "v0.0.1");

    PlayerCreationArguments playerCreationArguments{};
    bool printAvailablePlayers = false;
    unsigned int perftDepth = 0;
    unsigned int games = 1;
    unsigned int randomMoveDepthForStartState = 0;

    program.add_argument("-a", "--attacker")
        .store_into(playerCreationArguments.attacker)
        .help("Pass who should be the attacker, so the player without the king.");

    program.add_argument("-d", "--defender")
        .store_into(playerCreationArguments.defender)
        .help("Pass who should be the defender, so the player with the king.");

    program.add_argument("-t", "--time-to-think")
        .store_into(playerCreationArguments.timeToThink)
        .help("Pass how much the computer has to think.");

    program.add_argument("--printAvailablePlayers")
        .store_into(printAvailablePlayers)
        .help("Prints all player types that can be used.");

    program.add_argument("-p", "--perft")
        .scan<'i', unsigned int>()
        .store_into(perftDepth)
        .help("Run perft with a depth and print the output.");

    program.add_argument("-g", "--games")
        .scan<'i', unsigned int>()
        .store_into(games)
        .help("Run players play the passed amount of games against each other."
              "If more than one game is played the command line output is disabled and in the end "
              "a summary is printed.");

    program.add_argument("-r", "--random-start-state")
        .scan<'i', unsigned int>()
        .store_into(randomMoveDepthForStartState)
        .help("Start the game with a given amount of random moves made.");

    try {
        program.parse_args(argc, argv);
    } catch (const std::exception &err) {
        std::cerr << err.what() << std::endl;
        std::cerr << program;
        return 1;
    }

    std::unordered_set<std::string> availablePlayers = {"human", "random", "negamax",
                                                        "negamaxnoalphabeta"};

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

    if (playerCreationArguments.attacker.empty() || playerCreationArguments.defender.empty()) {
        throw std::invalid_argument("--attacker and --defender have to be passed");
    }
    if (!availablePlayers.contains(playerCreationArguments.attacker)) {
        throw std::invalid_argument(playerCreationArguments.attacker + " is not a known player.");
    }
    if (!availablePlayers.contains(playerCreationArguments.defender)) {
        throw std::invalid_argument(playerCreationArguments.defender + " is not a known player.");
    }

    PlayerFactory playerFactory(playerCreationArguments);
    if (games == 1) {
        Game game = createRandomGame(randomMoveDepthForStartState);
        SingleGameManager singleGameManager(game, playerFactory.createAttacker(),
                                            playerFactory.createDefender(), true);
        singleGameManager.run();
    } else {
        MultipleGameManager multipleGameManager(playerFactory, games, randomMoveDepthForStartState);
        multipleGameManager.run();
    }
}
