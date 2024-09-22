#include "SearchUtils.hpp"
#include "BitMasks.hpp"
#include "Game.hpp"

constexpr int KING_VALUE = 6;

auto evaluate(Game &game) -> int {
    InternalField field = game.getInternalField();
    const size_t wikingCount = (field & MASK_GET_ALL_WIKINGS_AND_KING).count() - 1;
    const size_t guardCount = (field & MASK_GET_ALL_GUARDS_AND_KING).count() - 1;

    return static_cast<int>(guardCount) + static_cast<int>(wikingCount) + KING_VALUE;
}
