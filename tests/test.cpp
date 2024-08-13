#include <catch2/catch_test_macros.hpp>

#include "Game.hpp"


TEST_CASE( "A move is tested", "[move]" ) {
    Game game;
    struct Position from = { 4, 1 };
    struct Position to = { 5, 1 };
    game.move(from, to);
    REQUIRE( game.figurAt(to) == Figur::Wiking );
}
