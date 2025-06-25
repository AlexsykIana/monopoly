#include "TaxTile.h"
#include "Player.h"
#include "GameBoard.h"
#include <iostream>

TaxTile::TaxTile(const std::string &tileName, BoardTile::TileType tileType, sf::Vector2f visualPos, int amount)
    : BoardTile(tileName, tileType, visualPos),
    taxAmount(amount)
{
    if (tileType != BoardTile::TileType::INCOME_TAX && tileType != BoardTile::TileType::LUXURY_TAX) {
        std::cout << "Warning WaxTile '" << tileName << "' created with a non-tax TileType: "
                  << static_cast<int>(tileType) << std::endl;
    }
}

int TaxTile::getTaxAmount() const {
    return taxAmount;
}

void TaxTile::onPlayerLanded(Player &player, GameBoard &board, std::vector<Player *> &allPlayers) {
    BoardTile::onPlayerLanded(player, board, allPlayers);

    std::cout << "Player " << player.getName() << " landed on " << this->getName()
              << " and must pay a tax of $" << taxAmount << std::endl;

    if (player.subtractMoney(taxAmount)) {
        std::cout << "$" << taxAmount << " paid by " << player.getName() << ". Remainig money: $" << player.getMoney() << std::endl;
    } else {
        std::cout << player.getName() << " cannot pay the tax of $" << taxAmount
                  << "! (Money: $" << player.getMoney() << "). Potential banckrupctcy." << std::endl;

        // Реалізувати метод банкруства
    }
}




