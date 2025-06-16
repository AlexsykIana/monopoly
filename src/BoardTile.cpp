#include "BoardTile.h"
#include "Player.h"
#include "GameBoard.h"
#include <iostream>

// Реалізація конструктора
BoardTile::BoardTile(const std::string& tileName, TileType tileType, sf::Vector2f visualPos)
    : name(tileName),
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