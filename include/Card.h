//
// Created by Legion on 16.06.2025.
//

#ifndef CARD_H
#define CARD_H

#include <string>
#include <vector>

class Player;
class GameBoard;

class Card {
    public:
    enum class ActionType {
        PAY_MONEY_TO_BANK,
        RECEIVE_MONEY_FROM_BANK,
        MOVE_TO_POSITION,       // Переміститися на певну клітинку (з проходженням GO або без)
        MOVE_FORWARD_SPACES,    // Переміститися вперед на N клітинок
        MOVE_TO_NEAREST_UTILITY,// Переміститися до найближчого комунального підприємства
        MOVE_TO_NEAREST_RAILROAD,// Переміститися до найближчої залізниці
        GO_TO_JAIL,             // Іти у в'язницю (не проходячи GO)
        GET_OUT_OF_JAIL_FREE,   // Картка "Вийти з в'язниці безкоштовно"
        REPAIRS_PER_HOUSE_HOTEL,// Ремонт: заплатити за кожен будинок/готель
        PAY_EACH_PLAYER,        // Заплатити кожному іншому гравцю
        RECEIVE_FROM_EACH_PLAYER// Отримати від кожного іншого гравця
    };

    private:
    std::string description;
    ActionType action;
    int value1;         // Перше значення, пов'язане з дією (наприклад, сума грошей, індекс клітинки, кількість кроків)
    int value2;         // Друге значення (наприклад, вартість за будинок, вартість за готель)
    bool isGetOutOfJailCard;

    public:
    Card(std::string desc, ActionType type, int value1 = 0, int value2 = 0, bool isGetOutOfJailCard = false);

    [[nodiscard]] const std::string& getDescription() const;
    [[nodiscard]] ActionType getActionType() const;
    [[nodiscard]] int getValue1() const;
    [[nodiscard]] int getValue2() const;
    [[nodiscard]] bool isGetOutOfJailFree() const;

    void applyEffect(Player& player, GameBoard& board, std::vector<Player*>& allPlayers) const;
};
#endif //CARD_H
