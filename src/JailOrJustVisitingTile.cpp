//
// Created by Legion on 18.06.2025.
//

#include "JailOrJustVisitingTile.h"
#include "Player.h"
#include "GameBoard.h"
#include <iostream>

JailOrJustVisitingTile::JailOrJustVisitingTile(sf::Vector2f visualPos)
    : BoardTile("Jail / Just Visiting", BoardTile::TileType::JAIL_OR_JUST_VISITING, visualPos) {
}

BoardTile::LandedAction JailOrJustVisitingTile::onPlayerLanded(Player &player, GameBoard &board, std::vector<Player *> &allPlayers) {
    BoardTile::onPlayerLanded(player, board, allPlayers);

    if (player.getIsInJail()) {
        std::cout << "Player " << player.getName() << " is currently in Jail." << std::endl;
    } else {
        std::cout << "Player " << player.getName() << " is just visiting Jail." << std::endl;
    }

    return LandedAction::NOTHING;
}
