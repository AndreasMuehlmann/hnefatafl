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
