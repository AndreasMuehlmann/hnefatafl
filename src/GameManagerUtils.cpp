#include "GameManagerUtils.hpp"
#include "Game.hpp"
#include "RandomMovesGenerator.hpp"

auto createRandomGame(size_t depth) -> Game {
    Game game;
    RandomMovesGenerator randomMovesGenerator;
    for (size_t i = 0; i < depth; i++) {
        Move move = randomMovesGenerator.getMove(game);
        if (game.makeMove(move) != Winner::NoWinner) {
            return createRandomGame(depth);
        }
    }
    return game;
}
