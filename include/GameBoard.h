// GameBoard.h
#ifndef GAME_BOARD_H
#define GAME_BOARD_H

#include <vector>
#include <SFML/Graphics.hpp>
#include <memory>
#include "BoardTile.h"

class GameBoard {
private:
    std::vector<std::unique_ptr<BoardTile>> tiles;

    const int TOTAL_SQUARES = 40;

    void initializeAllTiles();

public:

    GameBoard();

    [[nodiscard]] sf::Vector2f getVisualPositionForTile(int tileIndex) const;
    [[nodiscard]] const BoardTile* getTile(int tileIndex) const;
    [[nodiscard]] int getTotalSquares() const;

    // У майбутньому сюди можна буде додавати методи для:
    // - отримання типу клітинки за індексом
    // - обробки дії при потраплянні на клітинку (playerLandedOnSquare)
    // - роботи з колодами карток тощо.
};

#endif //GAME_BOARD_H