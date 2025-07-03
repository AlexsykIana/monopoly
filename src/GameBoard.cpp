#include "GameBoard.h"
#include "GoTile.h"
#include "StreetTile.h"
#include "Street.h"
#include "ChanceTile.h"
#include "CommunityChestTile.h"
#include "TaxTile.h"
#include "JailOrJustVisitingTile.h"
#include "GoToJailTile.h"
#include "FreeParkingTile.h"
#include "Player.h"
#include "Card.h"
#include <iostream>
#include <stdexcept>

GameBoard::GameBoard()
    : tiles(),
      chanceDeck(Deck::DeckType::CHANCE),
      communityChestDeck(Deck::DeckType::COMMUNITY_CHEST),
      jailPositionIndex(10),
      goPositionIndex(0) {
    initializeAllTiles();
    std::cout << "GameBoard created and initialized with " << tiles.size() << " tiles" << std::endl;
}

void GameBoard::initializeAllTiles() {
    tiles.reserve(TOTAL_SQUARES);

    using SCG = Street::StreetColorGroup;

    //0. GO
    tiles.emplace_back(std::make_unique<GoTile>(sf::Vector2f{840.f, 840.f}));

    // 1. Mediterranean Avenue (Brown)
    tiles.emplace_back(std::make_unique<StreetTile>(Street("Mediterranean Avenue", 60, SCG::BROWN, {745.f, 840.f})));

    // 2. Community Chest
    tiles.emplace_back(std::make_unique<CommunityChestTile>(sf::Vector2f{675.f, 840.f}));

    // 3. Baltic Avenue (Brown)
    tiles.emplace_back(std::make_unique<StreetTile>(Street("Baltic Avenue", 60, SCG::BROWN, {600.f, 840.f})));

    // 4. Income Tax
    tiles.emplace_back(std::make_unique<TaxTile>("Income Tax", BoardTile::TileType::INCOME_TAX,
                                                 sf::Vector2f(525.f, 840.f), 200));

    // 5. Reading Railroad (Train)
    tiles.emplace_back(std::make_unique<StreetTile>(Street("Reading Railroad", 200, SCG::TRAIN, {450.f, 840.f})));

    // 6. Oriental Avenue (Light Blue)
    tiles.emplace_back(std::make_unique<StreetTile>(Street("Oriental Avenue", 100, SCG::LIGHT_BLUE, {375.f, 840.f})));

    // 7. Chance
    tiles.emplace_back(std::make_unique<ChanceTile>(sf::Vector2f{300.f, 840.f}));

    // 8. Vermont Avenue (Light Blue)
    tiles.emplace_back(std::make_unique<StreetTile>(Street("Vermont Avenue", 100, SCG::LIGHT_BLUE, {230.f, 840.f})));

    // 9. Connecticut Avenue (Light Blue)
    tiles.emplace_back(
        std::make_unique<StreetTile>(Street("Connecticut Avenue", 120, SCG::LIGHT_BLUE, {155.f, 840.f})));

    // 10. Jail / Just Visiting
    tiles.emplace_back(std::make_unique<JailOrJustVisitingTile>(sf::Vector2f{60.f, 840.f}));

    // 11. St. Charles Place (Pink)
    tiles.emplace_back(std::make_unique<StreetTile>(Street("St. Charles Place", 140, SCG::PINK, {60.f, 745.f})));

    // 12. Electric Company (Communal)
    tiles.emplace_back(std::make_unique<StreetTile>(Street("Electric Company", 150, SCG::COMMUNAL, {60.f, 670.f})));

    // 13. States Avenue (Pink)
    tiles.emplace_back(std::make_unique<StreetTile>(Street("States Avenue", 140, SCG::PINK, {60.f, 600.f})));

    // 14. Virginia Avenue (Pink)
    tiles.emplace_back(std::make_unique<StreetTile>(Street("Virginia Avenue", 160, SCG::PINK, {60.f, 525.f})));

    // 15. Pennsylvania Railroad (Train)
    tiles.emplace_back(std::make_unique<StreetTile>(Street("Pennsylvania Railroad", 200, SCG::TRAIN, {60.f, 450.f})));

    // 16. St. James Place (Orange)
    tiles.emplace_back(std::make_unique<StreetTile>(Street("St. James Place", 180, SCG::ORANGE, {60.f, 375.f})));

    // 17. Community Chest
    tiles.emplace_back(std::make_unique<CommunityChestTile>(sf::Vector2f{60.f, 300.f}));

    // 18. Tennessee Avenue (Orange)
    tiles.emplace_back(std::make_unique<StreetTile>(Street("Tennessee Avenue", 180, SCG::ORANGE, {60.f, 230.f})));

    // 19. New York Avenue (Orange)
    tiles.emplace_back(std::make_unique<StreetTile>(Street("New York Avenue", 200, SCG::ORANGE, {60.f, 155.f})));

    // 20. Free Parking
    tiles.emplace_back(std::make_unique<FreeParkingTile>(sf::Vector2f{60.f, 60.f}));

    // 21. Kentucky Avenue (Red)
    tiles.emplace_back(std::make_unique<StreetTile>(Street("Kentucky Avenue", 220, SCG::RED, {155.f, 60.f})));

    // 22. Chance
    tiles.emplace_back(std::make_unique<ChanceTile>(sf::Vector2f{225.f, 60.f}));

    // 23. Indiana Avenue (Red)
    tiles.emplace_back(std::make_unique<StreetTile>(Street("Indiana Avenue", 220, SCG::RED, {305.f, 60.f})));

    // 24. Illinois Avenue (Red)
    tiles.emplace_back(std::make_unique<StreetTile>(Street("Illinois Avenue", 240, SCG::RED, {375.f, 60.f})));

    // 25. B. & O. Railroad (Train)
    tiles.emplace_back(std::make_unique<StreetTile>(Street("B. & O. Railroad", 200, SCG::TRAIN, {450.f, 60})));

    // 26. Atlantic Avenue (Yellow)
    tiles.emplace_back(std::make_unique<StreetTile>(Street("Atlantic Avenue", 260, SCG::YELLOW, {525.f, 60.f})));

    // 27. Ventnor Avenue (Yellow)
    tiles.emplace_back(std::make_unique<StreetTile>(Street("Ventnor Avenue", 260, SCG::YELLOW, {600.f, 60.f})));

    // 28. Water Works (Communal)
    tiles.emplace_back(std::make_unique<StreetTile>(Street("Water Works", 150, SCG::COMMUNAL, {670.f, 60.f})));

    // 29. Marvin Gardens (Yellow)
    tiles.emplace_back(std::make_unique<StreetTile>(Street("Marvin Gardens", 280, SCG::YELLOW, {745.f, 60.f})));

    // 30. Go To Jail
    tiles.emplace_back(std::make_unique<GoToJailTile>(sf::Vector2f{840.f, 60.f}));

    // 31. Pacific Avenue (Green)
    tiles.emplace_back(std::make_unique<StreetTile>(Street("Pacific Avenue", 300, SCG::GREEN, {840.f, 155.f})));

    // 32. North Carolina Avenue (Green)
    tiles.emplace_back(std::make_unique<StreetTile>(Street("North Carolina Avenue", 300, SCG::GREEN, {840.f, 230.f})));

    // 33. Community Chest
    tiles.emplace_back(std::make_unique<CommunityChestTile>(sf::Vector2f{840.f, 300.f}));

    // 34. Pennsylvania Avenue (Green)
    tiles.emplace_back(std::make_unique<StreetTile>(Street("Pennsylvania Avenue", 320, SCG::GREEN, {840.f, 375.f})));

    // 35. Short Line (Train)
    tiles.emplace_back(std::make_unique<StreetTile>(Street("Short Line", 200, SCG::TRAIN, {840.f, 450.f})));

    // 36. Chance
    tiles.emplace_back(std::make_unique<ChanceTile>(sf::Vector2f{840.f, 525.f}));

    // 37. Park Place (Blue)
    tiles.emplace_back(std::make_unique<StreetTile>(Street("Park Place", 350, SCG::BLUE, {840.f, 595.f})));

    // 38. Luxury Tax
    tiles.emplace_back(std::make_unique<TaxTile>("Luxury Tax", BoardTile::TileType::LUXURY_TAX,
                                                 sf::Vector2f(840.f, 670.f), 100));

    // 39. Boardwalk (Blue)
    tiles.emplace_back(std::make_unique<StreetTile>(Street("Boardwalk", 400, SCG::BLUE, {840.f, 745.f})));

    if (tiles.size() != TOTAL_SQUARES) {
        throw std::runtime_error("GameBoard::initializeAllTiles - Incorrect number of tiles initialized. Expected " +
                                 std::to_string(TOTAL_SQUARES) + ", got " + std::to_string(tiles.size()));
    }
}

BoardTile *GameBoard::getTileAt(int tileIndex) {
    if (tileIndex >= 0 && static_cast<size_t>(tileIndex) < tiles.size()) {
        return tiles[tileIndex].get();
    }
    std::cout << "Error: GameBoard::getTileAt - Attempted to access tile at invalid index :" << tileIndex << std::endl;
    return nullptr;
}

const BoardTile *GameBoard::getTileAt(int tileIndex) const {
    if (tileIndex >= 0 && static_cast<size_t>(tileIndex) < tiles.size()) {
        return tiles[tileIndex].get();
    }
    std::cerr << "Error: GameBoard::getTileAt (const) - Attempted to access tile at invalid index: " << tileIndex <<
            std::endl;
    return nullptr;
}

sf::Vector2f GameBoard::getVisualPositionForTile(int tileIndex) const {
    if (const BoardTile *tile = getTileAt(tileIndex)) {
        return tile->getVisualPosition();
    }
    std::cerr << "Error: GameBoard::getVisualPositionForTile - Could not get visual position for invalid tile index: "
            << tileIndex << std::endl;
    return {0.f, 0.f};
}

int GameBoard::getTotalSquares() {
    return TOTAL_SQUARES;
}

int GameBoard::getJailPosition() const {
    return jailPositionIndex;
}

int GameBoard::getGoPosition() const {
    return goPositionIndex;
}

int GameBoard::findTileIndexByStreet(const Street* streetToFind) const {
    if (!streetToFind) {
        return -1;
    }

    for (size_t i = 0; i < tiles.size(); ++i) {
        if (const auto* streetTile = dynamic_cast<const StreetTile*>(tiles[i].get())) {

            if (&(streetTile->getPropertyData()) == streetToFind) {
                return static_cast<int>(i);
            }
        }
    }

    return -1;
}

BoardTile::LandedAction GameBoard::playerLandedOnSquare(Player &player, int squareIndex, std::vector<Player *> &allPlayers) {
    if (BoardTile *tile = getTileAt(squareIndex)) {
        BoardTile::LandedAction result = tile->onPlayerLanded(player, *this, allPlayers);
        std::cout << "DEBUG GameBoard-Landed: Returning " << (int)result << std::endl;
        return result;
    } else {
        std::cout << "Error: GameBoard::playerLandedOnSquare * Player landed on a non-existent tile index: " <<
                squareIndex << std::endl;
    }
    return BoardTile::LandedAction::NOTHING;
}

BoardTile::LandedAction GameBoard::drawAndApplyChanceCard(Player &currentPlayer, std::vector<Player *> &allPlayers) {
    if (chanceDeck.isEmpty() && chanceDeck.discardPileIsEmpty()) {
        std::cout << "CHANCE DECK and discard pile are empty. Cannot draw card." << std::endl;
        return BoardTile::LandedAction::NOTHING;
    }
    Card drawCard = chanceDeck.drawCard(currentPlayer);
    BoardTile::LandedAction result = drawCard.applyEffect(currentPlayer, *this, allPlayers);
    std::cout << "DEBUG GameBoard: Returning " << (int)result << std::endl;
    return result;
}

BoardTile::LandedAction GameBoard::drawAndApplyCommunityChestCard(Player &currentPlayer, std::vector<Player *> &allPlayers) {
    if (communityChestDeck.isEmpty() && communityChestDeck.discardPileIsEmpty()) {
        std::cout << "COMMUNITY CHEST deck and discard pile are empty. Cannot draw card." << std::endl;
        return BoardTile::LandedAction::NOTHING;
    }
    Card drawCard = communityChestDeck.drawCard(currentPlayer);
    return drawCard.applyEffect(currentPlayer, *this, allPlayers);
}

int GameBoard::movePlayer(Player &player, int steps, bool &passedGo) {
    passedGo = false;

    if (player.getIsInJail()) {
        std::cout << player.getName() << " is in Jail. Standard movement by " << steps << " steps is not applicable." <<
                std::endl;
        return player.getCurrentPosition();
    }

    const int oldPosition = player.getCurrentPosition();
    int newPotentialPosition = oldPosition + steps;

    if (steps > 0 && newPotentialPosition >= TOTAL_SQUARES) {
        passedGo = true;
    }

    int newFinalPosition = newPotentialPosition % TOTAL_SQUARES;
    player.moveTo(newFinalPosition, TOTAL_SQUARES);


    if (passedGo) {
        std::cout << player.getName() << " passed GO this move." << std::endl;
    }

    return newFinalPosition;
}

void GameBoard::movePlayerToSquare(Player &player, int targetSquareIndex, bool cardSpecifiesGoSalary) {
    if (player.getIsInJail()) {
        std::cout << player.getName() << " is in Jail. Card movement to square " << targetSquareIndex
                << " might be restricted or have special rules." << std::endl;
        if (targetSquareIndex == getGoPosition() && cardSpecifiesGoSalary) {
            std::cout << player.getName() << " advances to GO from Jail (card effect)." << std::endl;
            player.leaveJail();
        } else {
            std::cout << player.getName() << " is in Jail. Card movement restricted." << std::endl;
            return;
        }
    }

    if (targetSquareIndex < 0 || targetSquareIndex >= TOTAL_SQUARES) {
        std::cout << "Error: GameBoard::movePlayerToSquare - Invalid target square index "
                << targetSquareIndex << " for moving player " << player.getName() << std::endl;
        return;
    }

    const int oldPosition = player.getCurrentPosition();
    bool passedGoThisMove = false;

    if (cardSpecifiesGoSalary) {
        if ((targetSquareIndex == getGoPosition() && oldPosition != getGoPosition()) || (
                oldPosition > targetSquareIndex)) {
            passedGoThisMove = true;
        }
    }


    player.moveTo(targetSquareIndex, TOTAL_SQUARES);

    if (const BoardTile *targetTile = getTileAt(targetSquareIndex)) {
        player.setTokenVisualPosition(targetTile->getVisualPosition());
    } else {
        std::cerr << "CRITICAL ERROR in GameBoard::movePlayerToSquare: Could not find target tile at index "
                << targetSquareIndex << " for player " << player.getName() << std::endl;
        player.moveTo(oldPosition, TOTAL_SQUARES);
        if (const BoardTile *oldTile = getTileAt(oldPosition)) {
            player.setTokenVisualPosition(oldTile->getVisualPosition());
        }
        return;
    }

    const BoardTile *finalTileInfo = getTileAt(targetSquareIndex);
    std::cout << player.getName() << " moved by card to "
            << (finalTileInfo ? finalTileInfo->getName() : "Unknown Square")
            << " (position " << targetSquareIndex << ")." << std::endl;

    if (passedGoThisMove) {
        std::cout << player.getName() << " passed GO and collects salary ($200)." << std::endl;
        player.addMoney(200);
    }
}
