#include "Card.h"
#include "Player.h"
#include "GameBoard.h"
#include <iostream>
#include <utility>


Card::Card(std::string desc, ActionType type, int val1, int val2, bool isGetOutOfJail)
    : description(std::move(desc)),
      action(type),
      value1(val1),
      value2(val2),
      isGetOutOfJailCard(isGetOutOfJail) {}

const std::string& Card::getDescription() const {
    return description;
}

Card::ActionType Card::getActionType() const {
    return action;
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

void Card::applyEffect(Player& player, GameBoard& board, std::vector<Player*>& allPlayers) const {
    std::cout << player.getName() << " drew card: " << description << std::endl;

    switch (action) {
        case ActionType::PAY_MONEY_TO_BANK: {
            std::cout << player.getName() << " pays " << value1 << " to the bank." << std::endl;
            player.subtractMoney(value1);
            if (!player.subtractMoney(value1)) {
                std::cout << player.getName() << " cannot afford to pay $" << value1 << ". Needs to manage assets or declare bankruptcy." << std::endl;
                // TODO: Логіка неможливості оплати
            }
            break;
        }

        case ActionType::RECEIVE_MONEY_FROM_BANK: {
            std::cout << player.getName() << " receives " << value1 << " from the bank." << std::endl;
            player.addMoney(value1);
            break;
        }

        case ActionType::MOVE_TO_POSITION: {
            int targetPosition = value1;
            const BoardTile* targetTileInfo = board.getTileAt(targetPosition);
            std::string targetName = targetTileInfo ? targetTileInfo->getName() : "Unknown Square";

            std::cout << player.getName() << " moves to " << targetName
                      << " (position " << targetPosition << ")." << std::endl;

            bool collectGo = true;

            board.movePlayerToSquare(player, targetPosition, collectGo);

            board.playerLandedOnSquare(player, player.getCurrentPosition(), allPlayers);
            break;
        }

        case ActionType::MOVE_FORWARD_SPACES: {
            int steps = value1;
            std::cout << player.getName() << " moves forward " << steps << " spaces." << std::endl;
            bool passedGo = false;
            int newPos = board.movePlayer(player, steps, passedGo);
            if(passedGo) {
                player.addMoney(200);
            }
            board.playerLandedOnSquare(player, newPos, allPlayers);
            break;
        }

        case ActionType::GO_TO_JAIL: {
            std::cout << player.getName() << " goes to jail." << std::endl;
            player.sendToJail(board.getJailPosition());
            if (const BoardTile* jailTile = board.getTileAt(board.getJailPosition())) {
                player.setTokenVisualPosition(jailTile->getVisualPosition());
            }
            break;
        }

        case ActionType::GET_OUT_OF_JAIL_FREE: {
            std::cout << player.getName() << " gets a 'Get Out of Jail Free' card." << std::endl;
            player.addGetOutOfJailFreeCard();
            break;
        }

        case ActionType::REPAIRS_PER_HOUSE_HOTEL: {
            int costPerHouse = value1;
            int costPerHotel = value2;
            int totalRepairCost = 0;
            for (const Street* street : player.getOwnedStreets()) {
                if (street) {
                    int houses = street->getHousesBuilt();
                    if (houses > 0 && houses < 5) {
                        totalRepairCost += houses * costPerHouse;
                    } else if (houses == 5) {
                        totalRepairCost += costPerHotel;
                    }
                }
            }
            std::cout << player.getName() << " pays $" << totalRepairCost << " for general repairs." << std::endl;
            if (!player.subtractMoney(totalRepairCost)) {
                std::cout << player.getName() << " cannot afford repairs. Needs to manage assets or declare bankruptcy." << std::endl;
                // TODO: Логіка неможливості оплати
            }
            break;
        }

        case ActionType::PAY_EACH_PLAYER: {
            int amountPerPlayer = value1;
            std::cout << player.getName() << " pays $" << amountPerPlayer << " to each other player." << std::endl;
            for (Player* otherPlayer : allPlayers) {
                if (otherPlayer != &player && !otherPlayer->getIsBankrupt()) {
                    if (player.subtractMoney(amountPerPlayer)) {
                        otherPlayer->addMoney(amountPerPlayer);
                        std::cout << "Paid $" << amountPerPlayer << " to " << otherPlayer->getName() << std::endl;
                    } else {
                        std::cout << player.getName() << " ran out of money paying " << otherPlayer->getName() << ". Stopping payments." << std::endl;
                        // TODO: Логіка неможливості оплати (банкрутство)
                        break;
                    }
                }
            }
            break;
        }

        case ActionType::RECEIVE_FROM_EACH_PLAYER: {
            int amountFromPlayer = value1;
            std::cout << player.getName() << " receives $" << amountFromPlayer << " from each other player." << std::endl;
            for (Player* otherPlayer : allPlayers) {
                if (otherPlayer != &player && !otherPlayer->getIsBankrupt()) {
                    if (otherPlayer->subtractMoney(amountFromPlayer)) {
                        player.addMoney(amountFromPlayer);
                        std::cout << "Received $" << amountFromPlayer << " from " << otherPlayer->getName() << std::endl;
                    } else {
                        int amountCanPay = otherPlayer->getMoney();
                        if(amountCanPay > 0) {
                            otherPlayer->subtractMoney(amountCanPay);
                            player.addMoney(amountCanPay);
                            std::cout << otherPlayer->getName() << " could only pay $" << amountCanPay << "." << std::endl;
                        } else {
                            std::cout << otherPlayer->getName() << " has no money to pay." << std::endl;
                        }
                        // TODO: Логіка, якщо інший гравець стає банкрутом
                    }
                }
            }
            break;
        }
        default:
            std::cout << "Unknown card action type for: " << description << std::endl;
            break;
    }
}