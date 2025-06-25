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
        getOutOfJailFreeCardsCount(0)
            {
                this->token.setRadius(15.f);
                this->token.setFillColor(tokenColor);
                this->token.setOutlineThickness(2.f);
                this->token.setOutlineColor(sf::Color::Black);
            }

const std::string& Player::getName() const {
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

bool Player::getIsInJail() const {
    return this->isInJail;
}

const sf::CircleShape& Player::getToken() const {
    return this->token;
}

bool Player::getIsBankrupt() const {
    return this->isBankrupt;
}

const std::vector<Street*>& Player::getOwnedStreets() const {
    return this->ownedStreets;
}

void Player::setName(const std::string& newName) {
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
    for (const Street* street : ownedStreets) {
        if (street && street->getGroup() == groupToCheck) {
            count++;
        }
    }
    return count == totalStreetsInGroup;
}

bool Player::useGetOutOfJailFreeCard() {
    if (getOutOfJailFreeCardsCount > 0) {
        getOutOfJailFreeCardsCount--;
        std::cout << name << " used a Get Out of Jail Free card. Remaining: " << getOutOfJailFreeCardsCount << std::endl;
        return true;
    }
    return false;
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

void Player::addOwnedStreet(Street* street) {
    if (street) {
        this->ownedStreets.push_back(street);
    }
}

void Player::removeOwnedStreet(const Street* streetToRemove) {
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
    for (Street* street : this->ownedStreets) {
        if (street) {
            street->clearOwner();
        }
    }
    this->ownedStreets.clear();
}

// Малює фішку гравця
void Player::drawToken(sf::RenderWindow& window) const {
    window.draw(this->token);
}