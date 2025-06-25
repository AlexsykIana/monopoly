#include "GoTile.h"
#include "Player.h"
#include <iostream>

#include "BoardTile.h"

GoTile::GoTile(sf::Vector2f visualPos)
    : BoardTile("GO", BoardTile::TileType::GO, visualPos) {}

void GoTile::onPlayerLanded(Player &player, GameBoard &board, std::vector<Player *> &allPlayers) {
    BoardTile::onPlayerLanded(player, board, allPlayers);

    std::cout << "Player " << player.getName() << " is at the square. Welcome!" << std::endl;

    constexpr int GO_LANDING_BONUS = 200;
    std::cout << "Player " << player.getName() << " receives an additional $"
               << GO_LANDING_BONUS << " for landing on GO." << std::endl;
    player.addMoney(GO_LANDING_BONUS);
}

