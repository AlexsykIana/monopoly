//
// Created by Legion on 18.06.2025.
//


#include "GoToJailTile.h"
#include "Player.h"
#include "GameBoard.h"
#include <iostream>

#include "BoardTile.h"

GoToJailTile::GoToJailTile(sf::Vector2f visualPos)
    : BoardTile("Go to Jail", BoardTile::TileType::GO_TO_JAIL, visualPos) {}

void GoToJailTile::onPlayerLanded(Player &player, GameBoard &board, std::vector<Player *> &allPlayers) {
    BoardTile::onPlayerLanded(player, board, allPlayers);

    std::cout << "Player " << player.getName() << " landed on 'Go to Jail'!" << std::endl;
    std::cout << "Sending " << player.getName() << " to Jail" << std::endl;

    player.sendToJail(board.getJailPosition());

    if (const BoardTile* jailSquare = board.getTileAt(board.getJailPosition())) {
        player.setTokenVisualPosition(jailSquare->getVisualPosition());
    } else {
        std::cout << "Error: Could not find Jail square on the board to move token!" << std::endl;
    }
}

