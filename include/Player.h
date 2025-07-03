//
// Created by Legion on 05.06.2025.
//

#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <set>
#include "Street.h"

class Street;
class Game;

class Player {
private:
    std::string name;
    int money;
    sf::CircleShape token;              // Фішка гравця на дошці (приклад візуального представлення)
    int currentPositionOnBoard;         // Поточна позиція на ігровій дошці (індекс клітинки)
    bool isInJail;                      // Чи знаходиться гравець у в'язниці
    int turnsInJail;                    // Кількість ходів, проведених у в'язниці
    std::vector<Street*> ownedStreets;  // Список вулиць, якими володіє гравець (вказівники на об'єкти Street)
    bool isBankrupt;
    int getOutOfJailFreeCardsCount;

public:
    Player(std::string playerName, int startMoney, sf::Color tokenColor);

    [[nodiscard]] const std::string& getName() const;
    [[nodiscard]] const std::vector<Street*>& getOwnedStreets() const;
    [[nodiscard]] const sf::CircleShape& getToken() const;
    [[nodiscard]] int getMoney() const;
    [[nodiscard]] int getCurrentPosition() const;
    [[nodiscard]] int getGetOutOfJailFreeCardsCount() const;
    [[nodiscard]] int getTurnsInJail() const;
    [[nodiscard]] bool getIsInJail() const;
    [[nodiscard]] bool getIsBankrupt() const;
    [[nodiscard]] bool ownsAllStreetsInGroup(Street::StreetColorGroup group, int totalStreetsInGroup) const;
    [[nodiscard]] bool canBuildOnStreet(const Street* street, const std::set<Street::StreetColorGroup>& builtGroups) const;
    [[nodiscard]] bool canSellHouseOnStreet(const Street* street) const;

    void addGetOutOfJailFreeCard();

    bool useGetOutOfJailFreeCard();

    void setName(const std::string& newName);

    void addMoney(int amount);

    bool subtractMoney(int amount);

    void moveTo(int newPosition, int boardSize);

    void setTokenVisualPosition(sf::Vector2f visualPos);

    void sendToJail(int jailPosition);

    void attemptToLeaveJail();

    void leaveJail();

    void incrementTurnsInJail();

    void addOwnedStreet(Street* street);

    void removeOwnedStreet(const Street* streetToRemove);

    void declareBankruptcy();

    void drawToken(sf::RenderWindow& window) const;
};

#endif //PLAYER_H
