//
// Created by Legion on 18.06.2025.
//

#ifndef TAXTILE_H
#define TAXTILE_H

#include "BoardTile.h"
#include <string>

class Player;

class TaxTile : public BoardTile {
private:
    int taxAmount;

public:
    TaxTile(const std::string& tileName, BoardTile::TileType tileType, sf::Vector2f visualPos, int amount);

    LandedAction onPlayerLanded(Player &player, GameBoard &board, std::vector<Player *> &allPlayers) override;

    [[nodiscard]] int getTaxAmount() const;
};
#endif //TAXTILE_H
