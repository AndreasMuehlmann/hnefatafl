#include "TestingUtils.hpp"

auto createGameFromField(Field &field, bool areWikingsToMove) -> Game {
    transform(field);
    Game game(field, areWikingsToMove);
    return game;
}

auto transform(Field &field) -> void {
    Field oldField = field;
    for (std::size_t x = 0; x < FIELD_SIZE; x++) {
        for (std::size_t y = 0; y < FIELD_SIZE; y++) {
            field.at(x).at(y) = oldField.at(y).at(x);
        }
    }
}
