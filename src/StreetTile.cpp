#include "StreetTile.h"
#include "Player.h"
#include "GameBoard.h"
#include "Game.h"
#include <iostream>

StreetTile::StreetTile(const Street &initialPropertyData)
    : BoardTile(initialPropertyData.getName(), // Назва для базового класу
                BoardTile::TileType::PROPERTY, // Тип клітинки для базового класу
                initialPropertyData.getPosOnBoard()), // Візуальна позиція для базового класу
      propertyData(initialPropertyData) {
}

Street &StreetTile::getPropertyData() {
    return propertyData;
}

const Street &StreetTile::getPropertyData() const {
    return propertyData;
}

BoardTile::LandedAction StreetTile::onPlayerLanded(Player &player, GameBoard &board, std::vector<Player *> &allPlayers) {
    BoardTile::onPlayerLanded(player, board, allPlayers);

    Player *currentOwner = propertyData.getOwner(); // получаєм поточного власника вулиці

    if (propertyData.getIsMortgaged()) {
        std::cout << propertyData.getName() << "is mortgaged. No rent to pay" << std::endl;
        return LandedAction::NOTHING;
    }

    if (currentOwner == nullptr) {
        std::cout << propertyData.getName() << "is unowned. Price: " << propertyData.getPurchasePrice() << std::endl;
        std::cout << "Player " << player.getName() << "(Money: " << player.getMoney() << ") can buy it" << std::endl;
    } else if (currentOwner != &player) {
        int rent = propertyData.getCurrentPenalty();
        std::cout << player.getName() << " must pay rent of $" << rent << " to "
                << currentOwner->getName() << " for " << propertyData.getName() << std::endl;

        if (player.subtractMoney(rent)) {
            currentOwner->addMoney(rent);
            std::cout << currentOwner->getName() << " received $ " << rent << " in rent " << std::endl;
        } else {
            std::cout << player.getName() << " cannot pay rent of $" << rent << "! (Potential bankruptcy)" << std::endl;
        }
    } else {
        std::cout << player.getName() << " landed on their won property: " << propertyData.getName() <<
                ". No rent to pay"
                << std::endl;
    }
    return LandedAction::NOTHING;
}
