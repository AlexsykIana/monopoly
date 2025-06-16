//
// Created by Legion on 29.05.2025.
//

#ifndef STREET_H
#define STREET_H

#include <SFML/Graphics.hpp>
#include <string>

class Player;

class Street {
public:
    enum class StreetColorGroup {
        BROWN, LIGHT_BLUE, PINK, ORANGE, RED, YELLOW, GREEN, BLUE, TRAIN, COMMUNAL
    };

    static constexpr  int STREETS_IN_BROWN_GROUP = 2;
    static constexpr  int STREETS_IN_LIGHT_BLUE_GROUP = 3;
    static constexpr  int STREETS_IN_PINK_GROUP = 3;
    static constexpr  int STREETS_IN_ORANGE_GROUP = 3;
    static constexpr  int STREETS_IN_RED_GROUP = 3;
    static constexpr  int STREETS_IN_YELLOW_GROUP = 3;
    static constexpr  int STREETS_IN_GREEN_GROUP = 3;
    static constexpr  int STREETS_IN_BLUE_GROUP = 2;
    static constexpr  int STREETS_IN_TRAIN_GROUP = 4;
    static constexpr  int STREETS_IN_COMMUNAL_GROUP = 2;

    static int getTotalStreetsInGroup(StreetColorGroup group);

private:
    std::string nameStreet;
    int purchasePrice;
    int currentPenalty;
    int housesBuilt;
    StreetColorGroup group;
    sf::Vector2f posOnBoard;

    bool isMortgaged;        // Чи закладена вулиця
    bool isOwned;            // Чи куплена вулиця взагалі
    Player* owner;           // Вказівник на гравця-власника (nullptr, якщо нічия)

    void updatePenalty();

public:
    Street(std::string name, int price, StreetColorGroup streetGroup, sf::Vector2f positionOnBoard);

    [[nodiscard]] const std::string& getName() const;
    [[nodiscard]] int getPurchasePrice() const;
    [[nodiscard]] int getCurrentPenalty() const;
    [[nodiscard]] bool getIsOwned() const;
    [[nodiscard]] Player* getOwner() const;
    [[nodiscard]] bool getIsMortgaged() const;
    [[nodiscard]] int getHousesBuilt() const;
    [[nodiscard]] StreetColorGroup getGroup() const;
    [[nodiscard]] sf::Vector2f getPosOnBoard() const;

    void clearOwner();
    bool buyStreet(Player* newOwner);
    int sellStreetToBank();
    bool buildHouse(Player* builder);
    bool mortgageStreet(const Player* mortgagor);
    bool redeemMortgage(Player* redeemer);
};

#endif //STREET_H
