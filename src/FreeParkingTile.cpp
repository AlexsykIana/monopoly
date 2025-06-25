//
// Created by Legion on 18.06.2025.
//

#include "FreeParkingTile.h"
#include "Player.h"
#include "GameBoard.h"
#include <iostream>

FreeParkingTile::FreeParkingTile(sf::Vector2f visualPos)
    : BoardTile("Free Parking", BoardTile::TileType::FREE_PARKING, visualPos) {}

void FreeParkingTile::onPlayerLanded(Player &player, GameBoard &board, std::vector<Player *> &allPlayers) {
    BoardTile::onPlayerLanded(player, board, allPlayers);

    std::cout << "Player " << player.getName() << " is restign at Free Parking. No action." << std::endl;
}

