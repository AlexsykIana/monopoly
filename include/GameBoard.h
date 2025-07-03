// GameBoard.h
#ifndef GAME_BOARD_H
#define GAME_BOARD_H

#include <vector>
#include <memory>
#include "BoardTile.h"
#include "Deck.h"

class Player;
class Street;

class GameBoard {
private:
    std::vector<std::unique_ptr<BoardTile>> tiles;

    Deck chanceDeck;
    Deck communityChestDeck;

    int jailPositionIndex;
    const int goPositionIndex;

    static constexpr int TOTAL_SQUARES = 40;

    void initializeAllTiles();

public:
    GameBoard();

    ~GameBoard() = default;

    [[nodiscard]] BoardTile* getTileAt(int tileIndex);
    [[nodiscard]] const BoardTile* getTileAt(int tileIndex) const;
    [[nodiscard]] sf::Vector2f getVisualPositionForTile(int tileIndex) const;
    [[nodiscard]] static int getTotalSquares();
    [[nodiscard]] int getJailPosition() const;
    [[nodiscard]] int getGoPosition() const;
    [[nodiscard]] int findTileIndexByStreet(const Street* streetToFind) const;

    BoardTile::LandedAction playerLandedOnSquare(Player& player, int squareIndex, std::vector<Player*>& allPlayers);
    BoardTile::LandedAction drawAndApplyChanceCard(Player& currentPlayer, std::vector<Player*>& allPlayers);
    BoardTile::LandedAction drawAndApplyCommunityChestCard(Player& currentPlayer, std::vector<Player*>& allPlayers);

    static int movePlayer(Player &player, int steps, bool &passedGo);
    void movePlayerToSquare(Player& player, int targetSquareIndex, bool collectGoSalaryIfPassed);
};

#endif //GAME_BOARD_H