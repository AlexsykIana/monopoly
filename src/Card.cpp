#include "Card.h"
#include "Player.h"
#include <iostream>


Card::Card(const std::string& desc, ActionType type, int val1, int val2, bool isGetOutOfJail)
    : description(desc),
      actionType(type),
      value1(val1),
      value2(val2),
      isGetOutOfJailCard(isGetOutOfJail) {}

const std::string& Card::getDescription() const {
    return description;
}

Card::ActionType Card::getActionType() const {
    return actionType;
}

int Card::getValue1() const {
    return value1;
}

int Card::getValue2() const {
    return value2;
}

bool Card::isGetOutOfJailFree() const {
    return isGetOutOfJailCard;
}

// Реалізація методу applyEffect
void Card::applyEffect(Player& player, GameBoard& board, std::vector<Player*>& allPlayers) {
    std::cout << player.getName() << " drew card: " << description << std::endl;

    switch (actionType) {
        case ActionType::PAY_MONEY_TO_BANK:
            std::cout << player.getName() << " pays " << value1 << " to the bank." << std::endl;
            player.subtractMoney(value1);
            break;

        case ActionType::RECEIVE_MONEY_FROM_BANK:
            std::cout << player.getName() << " receives " << value1 << " from the bank." << std::endl;
            player.addMoney(value1);
            break;

        case ActionType::MOVE_TO_POSITION:
            // Потрібно вирішити, чи проходить гравець "GO" і отримує зарплату.
            // value1 - цільова позиція.
            // Припустимо, board.movePlayerTo(player, value1, collectGoSalaryIfPassed);
            std::cout << player.getName() << " moves to position " << value1 << "." << std::endl;
            // board.movePlayerAndApplySquareEffect(player, value1, true /* чи проходити GO */); // Приклад
            player.moveTo(value1, board.getTotalSquares()); // Спрощено, треба врахувати GO
                                                            // і потім викликати логіку клітинки
            // board.playerLandedOn(player, player.getCurrentPosition()); // Потрібно буде викликати дію клітинки
            break;

        case ActionType::MOVE_FORWARD_SPACES:
            // value1 - кількість кроків
            std::cout << player.getName() << " moves forward " << value1 << " spaces." << std::endl;
            // int newPos = (player.getCurrentPosition() + value1) % board.getTotalSquares();
            // board.movePlayerAndApplySquareEffect(player, newPos, true);
            // player.moveTo(newPos, board.getTotalSquares()); // Потрібно врахувати GO
            // board.playerLandedOn(player, player.getCurrentPosition());
            break;

        // Складніші випадки, потребують більше інформації від GameBoard
        case ActionType::MOVE_TO_NEAREST_UTILITY:
        case ActionType::MOVE_TO_NEAREST_RAILROAD:
            // board.movePlayerToNearest(player, (actionType == ActionType::MOVE_TO_NEAREST_UTILITY ? TileType::UTILITY : TileType::RAILROAD));
            // На найближчому підприємстві/залізниці може бути особлива логіка оплати
            std::cout << "Action not yet fully implemented: " << description << std::endl;
            break;

        case ActionType::GO_TO_JAIL:
            std::cout << player.getName() << " goes to jail." << std::endl;
            // player.sendToJail(board.getJailPosition());
            break;

        case ActionType::GET_OUT_OF_JAIL_FREE:
            std::cout << player.getName() << " gets a 'Get Out of Jail Free' card." << std::endl;
            // player.addGetOutOfJailCard(); // Потрібен такий метод в Player
            // Ця картка не повертається в колоду відразу
            break;

        case ActionType::REPAIRS_PER_HOUSE_HOTEL:
            // value1 - вартість за будинок, value2 - вартість за готель
            // int totalRepairCost = 0;
            // for (Street* street : player.getOwnedStreets()) {
            //     if (street->getHousesBuilt() > 0 && street->getHousesBuilt() < 5) { // Будинки
            //         totalRepairCost += street->getHousesBuilt() * value1;
            //     } else if (street->getHousesBuilt() == 5) { // Готель
            //         totalRepairCost += value2;
            //     }
            // }
            // std::cout << player.getName() << " pays " << totalRepairCost << " for repairs." << std::endl;
            // player.subtractMoney(totalRepairCost);
            std::cout << "Action not yet fully implemented: " << description << std::endl;
            break;

        case ActionType::PAY_EACH_PLAYER:
            // value1 - сума кожному гравцю
            // for (Player* otherPlayer : allPlayers) {
            //     if (otherPlayer != &player && !otherPlayer->getIsBankrupt()) {
            //         if (player.subtractMoney(value1)) {
            //             otherPlayer->addMoney(value1);
            //         } else {
            //             // Обробка, якщо гравець не може заплатити всім
            //             break;
            //         }
            //     }
            // }
            std::cout << "Action not yet fully implemented: " << description << std::endl;
            break;

        case ActionType::RECEIVE_FROM_EACH_PLAYER:
            // value1 - сума від кожного гравця
            // for (Player* otherPlayer : allPlayers) {
            //     if (otherPlayer != &player && !otherPlayer->getIsBankrupt()) {
            //         if (otherPlayer->subtractMoney(value1)) {
            //             player.addMoney(value1);
            //         } else {
            //             // Обробка, якщо інший гравець не може заплатити
            //             // (можливо, отримує все, що може)
            //         }
            //     }
            // }
            std::cout << "Action not yet fully implemented: " << description << std::endl;
            break;

        default:
            std::cout << "Unknown card action type for: " << description << std::endl;
            break;
    }
}