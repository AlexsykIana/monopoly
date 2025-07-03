#include "CommunityChestTile.h"
#include "Player.h"
#include "GameBoard.h"
#include <iostream>

CommunityChestTile::CommunityChestTile(sf::Vector2f visualPos)
    : BoardTile("Community Chest", BoardTile::TileType::COMMUNITY_CHEST, visualPos) {
}


BoardTile::LandedAction CommunityChestTile::onPlayerLanded(Player &player, GameBoard &board, std::vector<Player *> &allPlayers) {
    std::cout << "Plaeyr" << player.getName() << "landed on Community Chest and draws a card .." << std::endl;
    board.drawAndApplyCommunityChestCard(player, allPlayers);
    return LandedAction::NOTHING;
}
