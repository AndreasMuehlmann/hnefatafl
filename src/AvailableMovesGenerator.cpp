#include "Figur.hpp"
#include "AvailableMovesGenerator.hpp"
#include "Game.hpp"
#include "GameUtils.hpp"
#include "Move.hpp"

AvailableMovesGenerator::AvailableMovesGenerator(const Game& game) :m_game(game), m_currentFigurPosition(0),
    m_positionDeltaIndex(0), m_currentTargetPositionForMove(0), m_isToMove(isAttacker) {}

auto AvailableMovesGenerator::next() -> std::optional<Move> {
    while (true) {
        while (( 0 < m_currentTargetPositionForMove && m_currentTargetPositionForMove < static_cast<int>(FIELDS))
            && m_game.getFigurAt(static_cast<Position>(m_currentTargetPositionForMove)) == Figur::NoFigur) {
            if ((m_positionDeltaIndex == 0 || m_positionDeltaIndex == 1) && static_cast<size_t>(m_currentTargetPositionForMove) / FIELD_SIZE != m_currentFigurPosition / FIELD_SIZE) {
                break;
            }
            Move move = {m_currentFigurPosition, static_cast<Position>(m_currentTargetPositionForMove)};
            m_currentTargetPositionForMove = m_currentTargetPositionForMove + POSITION_DELTAS.at(m_positionDeltaIndex);
            return move;
        }
        m_positionDeltaIndex++;
        if (m_positionDeltaIndex >= POSITION_DELTAS.size()) {
            if (!nextFigur()) {
                return std::nullopt;
            }
        } else {
            m_currentTargetPositionForMove = m_currentFigurPosition + POSITION_DELTAS.at(m_positionDeltaIndex);
        }
    }
}

auto AvailableMovesGenerator::nextFigur() -> bool {
    m_currentFigurPosition++;
    while (positionInBounds(m_currentFigurPosition)) {
        if (m_isToMove(m_game.getFigurAt(m_currentFigurPosition))) {
            m_positionDeltaIndex = 0;
            m_currentTargetPositionForMove = m_currentFigurPosition + POSITION_DELTAS.at(m_positionDeltaIndex);
            return true;
        }
        m_currentFigurPosition++;
    }
    return false;
}

auto AvailableMovesGenerator::reset() -> void {
    m_currentFigurPosition = 0;
    m_positionDeltaIndex = 0;
    m_currentTargetPositionForMove = 0;
    if (m_game.areAttackersToMove()) {
        m_isToMove = isAttacker;
    } else {
        m_isToMove = isDefender;
    }
    
    if (m_isToMove(m_game.getFigurAt(0))) {
        m_currentTargetPositionForMove = m_currentFigurPosition + POSITION_DELTAS.at(m_positionDeltaIndex);
    } else {
        nextFigur();
    }
}

