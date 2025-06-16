//
// Created by Legion on 05.06.2025.
//

#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "Street.h"

class Street;

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

public:
    Player(const std::string& playerName, int startMoney, sf::Color tokenColor);

    [[nodiscard]] const std::string& getName() const;
    [[nodiscard]] int getMoney() const;
    [[nodiscard]] int getCurrentPosition() const;
    [[nodiscard]] bool getIsInJail() const;
    [[nodiscard]] const sf::CircleShape& getToken() const;
    [[nodiscard]] bool getIsBankrupt() const;
    [[nodiscard]] const std::vector<Street*>& getOwnedStreets() const;
    [[nodiscard]] bool ownsAllStreetsInGroup(Street::StreetColorGroup group, int totalStreetsInGroup) const;


    void setName(const std::string& newName);

    void addMoney(int amount);

    bool subtractMoney(int amount);

    void moveTo(int newPosition, int boardSize);

    void setTokenVisualPosition(sf::Vector2f visualPos);

    void sendToJail(int jailPosition);

    void attemptToLeaveJail();

    void incrementTurnsInJail();

    // Додавання купленої вулиці
    void addOwnedStreet(Street* street);

    // Видалення вулиці зі списку володінь (наприклад, при продажу)
    void removeOwnedStreet(const Street* streetToRemove);

    // Оголошення банкрутства
    void declareBankruptcy();

    void drawToken(sf::RenderWindow& window) const;
};

#endif //PLAYER_H
