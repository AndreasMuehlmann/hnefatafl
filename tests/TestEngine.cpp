#include <catch2/catch_test_macros.hpp>
#include <vector>

#include "Game.hpp"
#include "Engine.hpp"
#include "helper.hpp"


TEST_CASE( "Test if all positions that are to move are collected", "[getAllFigursToMove]" ) {
    {
        Field field = {
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::Wiking, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::Wiking, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
        };
        transform(field);
        std::vector<Vec2D> figursToMove = getFigursToMove(field, true);
        REQUIRE(figursToMove[0].x == 0);
        REQUIRE(figursToMove[0].y == 5);
        REQUIRE(figursToMove[1].x == 1);
        REQUIRE(figursToMove[1].y == 0);
    }
    {
        Field field = {
            std::array<Figur, FIELD_SIZE>  {Figur::None, Figur::Guard, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::Guard, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
        };
        transform(field);
        std::vector<Vec2D> figursToMove = getFigursToMove(field, false);
        REQUIRE(figursToMove[0].x == 0);
        REQUIRE(figursToMove[0].y == 5);
        REQUIRE(figursToMove[1].x == 1);
        REQUIRE(figursToMove[1].y == 0);
    }
}

TEST_CASE( "Test getting available moves for a single figur in one direction", "[insertAvailableMovesFigurInDirection]" ) {
    Field field = {
        std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
        std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
        std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
        std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
        std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
        std::array<Figur, FIELD_SIZE> {Figur::Wiking, Figur::None, Figur::Guard, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
        std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
        std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
        std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
    };
    transform(field);

    std::vector<Move> availableMoves;
    insertAvailableMovesFigurInDirection(availableMoves, field, {0, 5}, {1, 0});
    REQUIRE(availableMoves.size() == 1);
    REQUIRE(availableMoves[0].from.x == 0);
    REQUIRE(availableMoves[0].from.y == 5);
    REQUIRE(availableMoves[0].to.x == 1);
    REQUIRE(availableMoves[0].to.y == 5);

    insertAvailableMovesFigurInDirection(availableMoves, field, {0, 5}, {-1, 0});
    REQUIRE(availableMoves.size() == 1);
    REQUIRE(availableMoves[0].to.x == 1);

    insertAvailableMovesFigurInDirection(availableMoves, field, {0, 5}, {0, 1});
    REQUIRE(availableMoves.size() == 3);
    REQUIRE(availableMoves[2].to.y == 7);

    insertAvailableMovesFigurInDirection(availableMoves, field, {0, 5}, {0, -1});
    REQUIRE(availableMoves.size() == 7);
    REQUIRE(availableMoves[6].to.y == 1);
}


TEST_CASE( "Test getting all available moves", "[getAvailableMoves]" ) {
    Field field = {
        std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
        std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
        std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
        std::array<Figur, FIELD_SIZE> {Figur::Guard, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
        std::array<Figur, FIELD_SIZE> {Figur::Wiking, Figur::King, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
        std::array<Figur, FIELD_SIZE> {Figur::Wiking, Figur::None, Figur::Guard, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
        std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
        std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
        std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
    };
    transform(field);
    std::vector<Vec2D> figursToMove = getFigursToMove(field, true);
    std::vector<Move> availableMoves = getAvailableMoves(field, figursToMove);
    REQUIRE(availableMoves[0].from.x == 0);
    REQUIRE(availableMoves[0].from.y == 5);
    REQUIRE(availableMoves[0].to.x == 1);
    REQUIRE(availableMoves[0].to.y == 5);
    REQUIRE(availableMoves.size() == 3);
    REQUIRE(availableMoves[2].to.y == 7);
}


TEST_CASE( "Test if correct moves are calcualted", "[minimax]" ) {
    {
        Field field = {
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::Wiking, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::Wiking, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::Wiking, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::Wiking},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::Guard, Figur::None, Figur::None, Figur::None, Figur::None, Figur::Wiking, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::Wiking, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::Wiking, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::King, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::Wiking, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::Wiking},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::Wiking, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::Wiking, Figur::None},
        };
        transform(field);

        Game game(field);
        game.move({{7, 8}, {6, 8}});
        Engine engine(game, 4);
        Move move = engine.getMove();
        // std::cout << "130: move " << move.from.x << ", " << move.from.y << "; " << move.to.x << ", " << move.to.y << std::endl;
        REQUIRE(move.from.y == 6);
        REQUIRE(move.from.x == 1);
        REQUIRE(move.to.x == 0);
        REQUIRE(move.to.y == 6);
    }
    {
        Field field = {
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::King, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::Wiking, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
        };
        transform(field);

        Game game(field);
        Engine engine(game, 4);
        game.move({{0, 3}, {0, 4}});
        Move move = engine.getMove();
        REQUIRE(move.from.x == 0);
        REQUIRE(move.from.y == 2);
        REQUIRE(move.to.x == 0);
        REQUIRE(move.to.y == 0);
    }
    {
        Field field = {
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::Wiking, Figur::None, Figur::None, Figur::None, Figur::King, Figur::Guard, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::Wiking, Figur::Guard, Figur::None, Figur::Wiking, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::Wiking, Figur::Wiking, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::Wiking},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::Wiking},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::Wiking},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::Wiking, Figur::Guard, Figur::None, Figur::None, Figur::Wiking},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::Wiking, Figur::None, Figur::None, Figur::Wiking, Figur::None, Figur::None, Figur::None},
        };
        transform(field);

        Game game(field);
        Engine engine(game, 4);
        game.move({{2, 8}, {3, 8}});
        Move move = engine.getMove();
        REQUIRE(move.from.x == 7);
        REQUIRE(move.from.y == 0);
        REQUIRE(move.to.x == 7);
        
    }
    {
        Field field = {
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::Wiking, Figur::None, Figur::None, Figur::None, Figur::None, Figur::Wiking},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::Wiking, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::Wiking, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::Wiking, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::Wiking, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::Wiking, Figur::None, Figur::Wiking},
            std::array<Figur, FIELD_SIZE> {Figur::King, Figur::Wiking, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::Guard, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::Wiking, Figur::Wiking, Figur::Wiking, Figur::None, Figur::Wiking, Figur::None},
        };
        transform(field);

        Game game(field);
        Engine engine(game, 4);
        game.move({{8, 1}, {8, 2}});
        Move move = engine.getMove();
        REQUIRE(move.from.x == 0);
        REQUIRE(move.from.y == 7);
        REQUIRE(move.to.x == 1);
        REQUIRE(move.to.y == 7);
        
    }
    {
        Field field = {
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::King, Figur::Wiking, Figur::None, Figur::None, Figur::None, Figur::Wiking, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::Wiking, Figur::Wiking, Figur::Guard, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::Wiking},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::Wiking, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::Wiking},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::Wiking, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::Wiking, Figur::None},
        };
        transform(field);

        Game game(field);
        Engine engine(game, 4);
        Move move = engine.getMove();
        REQUIRE(move.from.x == 1);
        REQUIRE(move.from.y == 1);
        REQUIRE(move.to.x == 1);
        REQUIRE(move.to.y == 0);
    }
}
