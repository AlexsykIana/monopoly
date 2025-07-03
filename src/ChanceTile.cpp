#include "ChanceTile.h"
#include "Player.h"
#include "GameBoard.h"
#include <iostream>


ChanceTile::ChanceTile(sf::Vector2f visualPos)
    : BoardTile("Chance", BoardTile::TileType::CHANCE, visualPos) {
}

BoardTile::LandedAction ChanceTile::onPlayerLanded(Player &player, GameBoard &board, std::vector<Player *> &allPlayers) {
    BoardTile::onPlayerLanded(player, board, allPlayers);

    std::cout << "Player " << player.getName() << " landed on Chance and draws a card..." << std::endl;

    LandedAction result = board.drawAndApplyChanceCard(player, allPlayers);
    std::cout << "DEBUG ChanceTile: Returning " << (int)result << std::endl;
    return result;
}
