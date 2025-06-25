#include "BoardTile.h"
#include "Player.h"
#include <iostream>
#include <utility>

BoardTile::BoardTile(std::string tileName, TileType tileType, sf::Vector2f visualPos)
    : name(std::move(tileName)),
      type(tileType),
      visualPosition(visualPos) {}


const std::string& BoardTile::getName() const {
    return name;
}

BoardTile::TileType BoardTile::getType() const {
    return type;
}

sf::Vector2f BoardTile::getVisualPosition() const {
    return visualPosition;
}


void BoardTile::onPlayerLanded(Player& player, GameBoard& board, std::vector<Player*>& allPlayers) {
    std::cout << "Player " << player.getName() << " landed on " << this->name
              << " (Type: " << static_cast<int>(this->type) << ")" << std::endl;
}