#include "Street.h"
#include "Player.h"

Street::Street(std::string name, int price, StreetColorGroup streetGroup, sf::Vector2f positionOnBoard)
    : nameStreet(std::move(name)),
      purchasePrice(price),
      currentPenalty(0),
      housesBuilt(0),
      group(streetGroup),
      posOnBoard(positionOnBoard),
      isMortgaged(false),
      isOwned(false),
      owner(nullptr) {
    updatePenalty();
}

const std::string &Street::getName() const {
    return this->nameStreet;
}

int Street::getPurchasePrice() const {
    return this->purchasePrice;
}

int Street::getCurrentPenalty() const {
    return this->currentPenalty;
}

bool Street::getIsOwned() const {
    return this->isOwned;
}

Player *Street::getOwner() const {
    return this->owner;
}

bool Street::getIsMortgaged() const {
    return this->isMortgaged;
}

int Street::getHousesBuilt() const {
    return this->housesBuilt;
}

Street::StreetColorGroup Street::getGroup() const {
    return this->group;
}

sf::Vector2f Street::getPosOnBoard() const {
    return this->posOnBoard;
}

void Street::clearOwner() {
    this->owner = nullptr;
    this->isOwned = false;
    updatePenalty();
}

int Street::getHouseCost() const {
    switch (group) {
        case StreetColorGroup::BROWN:
        case StreetColorGroup::LIGHT_BLUE: return 50;
        case StreetColorGroup::PINK:
        case StreetColorGroup::ORANGE: return 100;
        case StreetColorGroup::RED:
        case StreetColorGroup::YELLOW: return 150;
        case StreetColorGroup::GREEN:
        case StreetColorGroup::BLUE: return 200;
        default: return 0;
    }
}

int Street::getTotalStreetsInGroup(StreetColorGroup group) {
    switch (group) {
        case StreetColorGroup::BROWN: return STREETS_IN_BROWN_GROUP;
        case StreetColorGroup::LIGHT_BLUE: return STREETS_IN_LIGHT_BLUE_GROUP;
        case StreetColorGroup::PINK: return STREETS_IN_PINK_GROUP;
        case StreetColorGroup::ORANGE: return STREETS_IN_ORANGE_GROUP;
        case StreetColorGroup::RED: return STREETS_IN_RED_GROUP;
        case StreetColorGroup::YELLOW: return STREETS_IN_YELLOW_GROUP;
        case StreetColorGroup::GREEN: return STREETS_IN_GREEN_GROUP;
        case StreetColorGroup::BLUE: return STREETS_IN_BLUE_GROUP;
        case StreetColorGroup::TRAIN: return STREETS_IN_TRAIN_GROUP;
        case StreetColorGroup::COMMUNAL: return STREETS_IN_COMMUNAL_GROUP;
        default: return 0;
    }
}

void Street::updatePenalty() {
    if (!isOwned || isMortgaged) {
        currentPenalty = 0;
    } else {
        const int baseRent = purchasePrice / 10;
        if (housesBuilt == 0) {
            currentPenalty = baseRent;
        } else if (housesBuilt == 1) {
            currentPenalty = baseRent * 2;
        } else if (housesBuilt == 2) {
            currentPenalty = baseRent * 3;
        } else if (housesBuilt == 3) {
            currentPenalty = baseRent * 4;
        } else if (housesBuilt == 4) {
            currentPenalty = baseRent * 5;
        } else if (housesBuilt == 5) {
            currentPenalty = baseRent * 10;
        }
    }
}

bool Street::buyStreet(Player *newOwner) {
    if (!isOwned && newOwner && newOwner->subtractMoney(purchasePrice)) {
        this->owner = newOwner;
        this->isOwned = true;
        newOwner->addOwnedStreet(this);
        updatePenalty();
        return true;
    }
    return false;
}

int Street::sellStreetToBank() {
    if (isOwned && !isMortgaged && housesBuilt == 0) {
        const int sellPrice = purchasePrice / 2;
        this->owner = nullptr;
        this->isOwned = false;
        this->housesBuilt = 0;
        updatePenalty();
        return sellPrice;
    }
    return 0;
}

int Street::sellHouse () {
    if (housesBuilt == 0) { return 0; }
    int sellPrice = getHouseCost() / 2;
    housesBuilt--;
    updatePenalty();
    return sellPrice;
}

bool Street::buildHouse(Player *builder) {
    if (isOwned && owner == builder && !isMortgaged && housesBuilt < 5 /*max houses*/) {
        int costOfHouse = 0;
        switch (this->group) {
            case StreetColorGroup::BROWN:
            case StreetColorGroup::LIGHT_BLUE:
                costOfHouse = 50;
                break;
            case StreetColorGroup::PINK:
            case StreetColorGroup::ORANGE:
                costOfHouse = 100;
                break;
            case StreetColorGroup::RED:
            case StreetColorGroup::YELLOW:
                costOfHouse = 150;
                break;
            case StreetColorGroup::GREEN:
            case StreetColorGroup::BLUE:
                costOfHouse = 200;
                break;
            case StreetColorGroup::TRAIN:
            case StreetColorGroup::COMMUNAL:
                return false;
        }
        if (costOfHouse == 0) {
            return false;
        }
        if (this->group != StreetColorGroup::TRAIN && this->group != StreetColorGroup::COMMUNAL) {
            int totalInGroup = getTotalStreetsInGroup(this->group);
            if (!builder->ownsAllStreetsInGroup(this->group, totalInGroup)) {
                return false;
            }
        }
        if (builder->subtractMoney(costOfHouse)) {
            housesBuilt++;
            updatePenalty();
            return true;
        } else {
            return false;
        }
    }
    return false;
}

bool Street::mortgageStreet(const Player *mortgagor) {
    if (isOwned && owner == mortgagor && !isMortgaged && housesBuilt == 0) {
        isMortgaged = true;
        updatePenalty();
        const int moneyForMortgage = purchasePrice / 2;
        owner->addMoney(moneyForMortgage);
        return true;
    }
    return false;
}

bool Street::redeemMortgage(Player *redeemer) {
    if (isOwned && owner == redeemer && isMortgaged) {
        int moneyForRedeemMortgageMortgage = (purchasePrice / 2);
        moneyForRedeemMortgageMortgage = static_cast<int>(moneyForRedeemMortgageMortgage * 1.10);
        if (redeemer->subtractMoney(moneyForRedeemMortgageMortgage)) {
            isMortgaged = false;
            updatePenalty();
            return true;
        } else {
            return false;
        }
    }
    return false;
}
