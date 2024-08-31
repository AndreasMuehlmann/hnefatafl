#include <cstring>
#include <iostream>
#include <stdexcept>

#include "Game.hpp"
#include "Utils.hpp"

constexpr unsigned int max_wiking_count = 16;
constexpr unsigned int max_guard_count = 16;

Game::Game()
    : m_field(), m_wikingsToMove(true),
      m_kingPosition({(FIELD_SIZE - 1) / 2, (FIELD_SIZE - 1) / 2}), m_guardCount(max_guard_count),
      m_wikingCount(max_wiking_count) {
    constexpr Figur w = Figur::Wiking;
    constexpr Figur g = Figur::Guard;
    constexpr Figur k = Figur::King;
    constexpr Figur _ = Figur::None;
    m_field = {
        std::array<Figur, FIELD_SIZE>{_, _, _, w, w, w, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, w, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, g, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{w, _, _, _, g, _, _, _, w},
        std::array<Figur, FIELD_SIZE>{w, w, g, g, k, g, g, w, w},
        std::array<Figur, FIELD_SIZE>{w, _, _, _, g, _, _, _, w},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, g, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, w, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, w, w, w, _, _, _},
    };
}

Game::Game(Field field, bool wikingsToMove)
    : m_field(field), m_wikingsToMove(wikingsToMove), m_kingPosition(), m_guardCount(0),
      m_wikingCount(0) {
    unsigned int kingCount = 0;
    for (size_t x = 0; x < FIELD_SIZE; x++) {
        for (size_t y = 0; y < FIELD_SIZE; y++) {
            Vec2D position = {static_cast<int>(x), static_cast<int>(y)};
            if (getFigurAt(position) == Figur::Wiking) {
                m_wikingCount += 1;
            } else if (getFigurAt(position) == Figur::Guard) {
                m_guardCount += 1;
            } else if (getFigurAt(position) == Figur::King) {
                kingCount += 1;
                if (kingCount >= 2) {
                    throw std::invalid_argument("Two kings in field.");
                }
                m_kingPosition = position;
            }
        }
    }
}

auto Game::areWikingsToMove() const -> bool { return m_wikingsToMove; }

auto Game::getFigurAt(Vec2D position) const -> Figur {
    return m_field.at(static_cast<size_t>(position.x)).at(static_cast<size_t>(position.y));
}

auto Game::setFigurAt(Figur figur, Vec2D position) -> void {
    m_field.at(static_cast<size_t>(position.x)).at(static_cast<size_t>(position.y)) = figur;
}

auto Game::getField() const -> const Field & { return m_field; }

auto Game::getWikingCount() const -> unsigned int { return m_wikingCount; }

auto Game::getGuardCount() const -> unsigned int { return m_guardCount; }

auto Game::isPositionInBounds(Vec2D position) -> bool {
    return 0 <= position.x && static_cast<size_t>(position.x) < FIELD_SIZE && 0 <= position.y &&
           static_cast<size_t>(position.y) < FIELD_SIZE;
}

auto Game::moveUnchecked(Move move) -> void {
    Figur figur = getFigurAt(move.from);
    setFigurAt(figur, move.to);
    setFigurAt(Figur::None, move.from);
    if (figur == Figur::King) {
        m_kingPosition = move.to;
    }

    m_wikingsToMove = !m_wikingsToMove;
}

auto Game::isBlocked(Move move) const -> bool {
    Vec2D direction = {sign(move.to.x - move.from.x), sign(move.to.y - move.from.y)};
    Vec2D positionToCheck = move.from;
    while ((positionToCheck.x != move.to.x || positionToCheck.y != move.to.y) &&
           isPositionInBounds(positionToCheck)) {
        positionToCheck = {positionToCheck.x + direction.x, positionToCheck.y + direction.y};
        if (getFigurAt(positionToCheck) != Figur::None) {
            return true;
        }
    }
    return false;
}

auto Game::move(Move move) -> void {
    if (!Game::isPositionInBounds(move.from)) {
        throw std::invalid_argument("Position to move away from out of field range.");
    }
    if (!Game::isPositionInBounds(move.to)) {
        throw std::invalid_argument("Position to move to out of field range.");
    }
    if (move.from.x == move.to.x && move.from.y == move.to.y) {
        throw std::invalid_argument("Position to move to and position to move from are equal.");
    }

    Figur figur = getFigurAt(move.from);
    if (figur == Figur::None) {
        throw std::invalid_argument("On the position to move away from is no figur.");
    }
    if ((m_wikingsToMove && isKingDefender(getFigurAt(move.from))) ||
        (!m_wikingsToMove && isKingAttacker(getFigurAt(move.from)))) {
        throw std::invalid_argument("The figur belongs to the other player.");
    }
    if (move.from.x != move.to.x && move.from.y != move.to.y) {
        throw std::invalid_argument("Diagonal movement is not allowed.");
    }
    if ((move.to.x == 0 || move.to.x == FIELD_SIZE - 1) &&
        (move.to.y == 0 || move.to.y == FIELD_SIZE - 1) && figur != Figur::King) {
        throw std::invalid_argument("Cannot move into the corner unless the figur is the king.");
    }
    if ((move.to.x == (FIELD_SIZE - 1) / 2 && move.to.y == (FIELD_SIZE - 1) / 2) &&
        figur != Figur::King) {
        throw std::invalid_argument("Cannot move into the center position "
                                    "unless the figur is the king.");
    }

    if (!isBlocked(move)) {
        setFigurAt(figur, move.to);
        setFigurAt(Figur::None, move.from);
        if (figur == Figur::King) {
            m_kingPosition = move.to;
        }
    } else {
        throw std::invalid_argument("Cannot move because the path is blocked.");
    }

    m_wikingsToMove = !m_wikingsToMove;
}

auto Game::capture(Vec2D lastMovedTo, Vec2D direction) -> void {
    Vec2D positionToCheck = {lastMovedTo.x + direction.x, lastMovedTo.y + direction.y};
    Figur movedFigur = getFigurAt(lastMovedTo);
    if (!isPositionInBounds(positionToCheck)) {
        return;
    }
    if (movedFigur == getFigurAt(positionToCheck) ||
        (movedFigur == Figur::Guard && getFigurAt(positionToCheck) == Figur::King)) {
        return;
    }
    while (isPositionInBounds(positionToCheck)) {
        Figur figur = getFigurAt(positionToCheck);
        if (figur == Figur::None || (movedFigur == Figur::Wiking && figur == Figur::King)) {
            break;
        }
        if ((movedFigur == Figur::Wiking && isKingAttacker(figur)) ||
            (movedFigur != Figur::Wiking && isKingDefender(figur))) {
            Vec2D positionToDelete = {lastMovedTo.x + direction.x, lastMovedTo.y + direction.y};
            while (positionToDelete.x != positionToCheck.x ||
                   positionToDelete.y != positionToCheck.y) {
                Figur figurToDelete = getFigurAt(positionToDelete);
                if (figurToDelete == Figur::Guard) {
                    m_guardCount -= 1;
                } else if (figurToDelete == Figur::Wiking) {
                    m_wikingCount -= 1;
                }
                setFigurAt(Figur::None, positionToDelete);
                positionToDelete = {positionToDelete.x + direction.x,
                                    positionToDelete.y + direction.y};
            }
            break;
        }
        positionToCheck = {positionToCheck.x + direction.x, positionToCheck.y + direction.y};
    }
}

auto Game::updateField(Vec2D lastMovedTo) -> void {
    capture(lastMovedTo, {1, 0});
    capture(lastMovedTo, {-1, 0});
    capture(lastMovedTo, {0, 1});
    capture(lastMovedTo, {0, -1});
}

auto Game::whoWon() const -> Figur {
    if ((m_kingPosition.x == 0 || m_kingPosition.x == FIELD_SIZE - 1) &&
        (m_kingPosition.y == 0 || m_kingPosition.y == FIELD_SIZE - 1)) {
        return Figur::King;
    }
    if ((m_kingPosition.x == FIELD_SIZE - 1 ||
         getFigurAt({m_kingPosition.x + 1, m_kingPosition.y}) == Figur::Wiking) &&
        (m_kingPosition.x == 0 ||
         getFigurAt({m_kingPosition.x - 1, m_kingPosition.y}) == Figur::Wiking) &&
        (m_kingPosition.y == FIELD_SIZE - 1 ||
         getFigurAt({m_kingPosition.x, m_kingPosition.y + 1}) == Figur::Wiking) &&
        (m_kingPosition.y == 0 ||
         getFigurAt({m_kingPosition.x, m_kingPosition.y - 1}) == Figur::Wiking)) {
        return Figur::Wiking;
    }
    return Figur::None;
}

auto Game::printField() const -> void {
    for (size_t y = 0; y < FIELD_SIZE; y++) {
        for (size_t x = 0; x < FIELD_SIZE; x++) {
            Vec2D position = {static_cast<int>(x), static_cast<int>(y)};
            switch (getFigurAt(position)) {
            case Figur::None:
                std::cout << "_";
                break;
            case Figur::Wiking:
                std::cout << "W";
                break;
            case Figur::Guard:
                std::cout << "G";
                break;
            case Figur::King:
                std::cout << "K";
                break;
            }
        }
        std::cout << std::endl;
    }
}
