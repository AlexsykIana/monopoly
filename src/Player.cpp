#include "Player.h"
#include "Game.h"
#include <iostream>
#include <utility>

Player::Player(std::string playerName, int startMoney, sf::Color tokenColor)
    : name(std::move(playerName)),
      money(startMoney),
      currentPositionOnBoard(0),
      isInJail(false),
      turnsInJail(0),
      isBankrupt(false),
      getOutOfJailFreeCardsCount(0) {
    float radius = 15.f;
    this->token.setRadius(radius);
    this->token.setFillColor(tokenColor);
    this->token.setOutlineThickness(2.f);
    this->token.setOutlineColor(sf::Color::Black);
    this->token.setOrigin(radius, radius);  // центруєм фішку гравця
}

const std::string &Player::getName() const {
    return this->name;
}

int Player::getMoney() const {
    return this->money;
}

int Player::getCurrentPosition() const {
    return this->currentPositionOnBoard;
}

int Player::getGetOutOfJailFreeCardsCount() const {
    return getOutOfJailFreeCardsCount;
}

int Player::getTurnsInJail() const {
    return turnsInJail;
}

bool Player::getIsInJail() const {
    return this->isInJail;
}

bool Player::useGetOutOfJailFreeCard() {
    if (getOutOfJailFreeCardsCount > 0) {
        getOutOfJailFreeCardsCount--;
        return true;
    }
    return false;
}
const sf::CircleShape &Player::getToken() const {
    return this->token;
}

bool Player::getIsBankrupt() const {
    return this->isBankrupt;
}

const std::vector<Street *> &Player::getOwnedStreets() const {
    return this->ownedStreets;
}

void Player::setName(const std::string &newName) {
    this->name = newName;
}

void Player::addMoney(const int amount) {
    if (amount > 0) {
        this->money += amount;
    }
}

bool Player::subtractMoney(const int amount) {
    if (amount < 0) return true;
    if (this->money >= amount) {
        this->money -= amount;
        return true;
    }
    return false;
}

bool Player::ownsAllStreetsInGroup(Street::StreetColorGroup groupToCheck, int totalStreetsInGroup) const {
    if (totalStreetsInGroup == 0) return false;

    int count = 0;
    for (const Street *street: ownedStreets) {
        if (street && street->getGroup() == groupToCheck) {
            count++;
        }
    }
    return count == totalStreetsInGroup;
}

bool Player::canBuildOnStreet(const Street* street, const std::set<Street::StreetColorGroup>& builtGroups) const {
    if (!street || street->getIsMortgaged() || street->getOwner() != this) { return false; }

    Street::StreetColorGroup group = street->getGroup();

    if (group == Street::StreetColorGroup::TRAIN || group == Street::StreetColorGroup::COMMUNAL) {
        return false;
    }

    int totalInGroup = Street::getTotalStreetsInGroup(group);
    if (!ownsAllStreetsInGroup(group, totalInGroup)) {
        return false;
    }

    int houses_on_target_street = street->getHousesBuilt();
    if (houses_on_target_street >= 5) {
        return false;
    }

    for (const Street* other_street : ownedStreets) {
        if (other_street && other_street->getGroup() == group) {
            if (other_street->getHousesBuilt() < houses_on_target_street) {
                return false;
            }
        }
    }

    if (builtGroups.count(group) > 0) {
        return false; // Вже будували, більше не можна
    }

    return true;
}

bool Player::canSellHouseOnStreet(const Street *street) const {
    if (!street || street->getIsMortgaged() || street->getOwner() != this || street->getHousesBuilt() == 0) { return false; }

    Street::StreetColorGroup group = street->getGroup();
    const int totalInGroup = Street::getTotalStreetsInGroup(group);
    const int totalHouseOnThisStreet = street->getHousesBuilt();

    for (const Street *otherStree: ownedStreets) {
        if (otherStree && otherStree->getGroup() == group) {
            if (otherStree->getHousesBuilt() > totalHouseOnThisStreet) {
                return false;
            }
        }
    }
    return true;
}


void Player::addGetOutOfJailFreeCard() {
    getOutOfJailFreeCardsCount++;
    std::cout << name << " now has " << getOutOfJailFreeCardsCount << " Get Out of Jail Free card(s)." << std::endl;
}

void Player::moveTo(int newPosition, int boardSize) {
    if (newPosition < this->currentPositionOnBoard && !this->isInJail) {
        // Можливо, тут має бути логіка нарахування грошей за проходження "GO"
        // addMoney(SALARY_FOR_GO);
    }
    this->currentPositionOnBoard = newPosition % boardSize;
}

// Встановлює візуальну позицію фішки гравця на екрані
void Player::setTokenVisualPosition(sf::Vector2f visualPos) {
    this->token.setPosition(visualPos);
}

// Відправляє гравця до в'язниці, оновлюючи його стан та позицію.
void Player::sendToJail(int jailPosition) {
    this->isInJail = true;
    this->turnsInJail = 0;
    this->currentPositionOnBoard = jailPosition;
}

void Player::leaveJail() {
    if (isInJail) {
        isInJail = false;
        turnsInJail = 0;
        std::cout << name << " is now out of jail." << std::endl;
    }
}

// Спроба вийти з в'язниці (Треба буде дописати)
void Player::attemptToLeaveJail() {
    // Тут буде логіка спроби виходу (наприклад, кидок кубиків, оплата, картка)
    // Поки що просто вихід:
    this->isInJail = false;
    this->turnsInJail = 0;
}

// Збільшує лічильник ходів, проведених гравцем у в'язниці.
void Player::incrementTurnsInJail() {
    if (this->isInJail) {
        this->turnsInJail++;
    }
}

void Player::addOwnedStreet(Street *street) {
    if (street) {
        this->ownedStreets.push_back(street);
    }
}

void Player::removeOwnedStreet(const Street *streetToRemove) {
    for (auto it = this->ownedStreets.begin(); it != this->ownedStreets.end(); ++it) {
        if (*it == streetToRemove) {
            this->ownedStreets.erase(it);
            return;
        }
    }
}

// Оголошує гравця банкрутом: обнуляє гроші, очищає список вулиць.(Треба буде дописати)
void Player::declareBankruptcy() {
    this->isBankrupt = true;
    this->money = 0;
    for (Street *street: this->ownedStreets) {
        if (street) {
            street->clearOwner();
        }
    }
    this->ownedStreets.clear();
}

// Малює фішку гравця
void Player::drawToken(sf::RenderWindow &window) const {
    window.draw(this->token);
}
