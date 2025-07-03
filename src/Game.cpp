//
// Created by Legion on 19.06.2025.
//

#include "Game.h"
#include "BoardTile.h"
#include "StreetTile.h"
#include <iostream>
#include <vector>
#include <string>

Game::Game()
    : currentGameState(GameState::NORMAL_TURN),
      stateBeforeConfirmation(GameState::NORMAL_TURN),
      selectedStreet(nullptr),
      randomEngine(std::random_device{}()),
      diceDistribution(1, 6),
      uiManager(nullptr),
      maxLogMessages(150),
      logCharacterSize(14),
      chatScrollOffset(0.f),
      totalLogTextHeight(0.f),
      boardActualWidth(0.f),
      uiPanelX(0.f),
      currentPlayerIndex(0),
      doublesRolledCount(0),
      isRunning(false),
      rollDoneThisTurn(false),
      playerJustLanded(false) {
    if (!loadResources()) {
        std::cout << "Failed to load resources!" << std::endl;
        return;
    }

    this->boardActualWidth = static_cast<float>(boardTexture.getSize().x);
    this->uiPanelX = this->boardActualWidth + PADDING;

    auto windowWidth = static_cast<unsigned int>(this->boardActualWidth + PADDING + Game::UI_AREA_WIDTH);
    auto windowHeight = boardTexture.getSize().y;

    window.create(sf::VideoMode(windowWidth, windowHeight), "Monopoly by Nazar");
    window.setVerticalSyncEnabled(true);

    boardSprite.setTexture(boardTexture);
    boardSprite.setPosition(0.f, 0.f);

    float buttonsStartY = PADDING + CHAT_AREA_HEIGHT + PADDING + PLAYER_STATUS_AREA_HEIGHT + PADDING;
    uiManager = std::make_unique<UIManager>(mainFont, this->uiPanelX, buttonsStartY);

    selectionHighlight.setFillColor(sf::Color(255, 215, 0, 100));
    selectionHighlight.setOutlineColor(sf::Color::White);
    selectionHighlight.setOutlineThickness(2.f);

    confirmationDialogBackground.setSize(sf::Vector2f(350, 150));
    confirmationDialogBackground.setFillColor(sf::Color(40, 40, 40, 240));
    confirmationDialogBackground.setOutlineColor(sf::Color::White);
    confirmationDialogBackground.setOutlineThickness(2.f);
    // Позицію будемо встановлювати в центрі екрану динамічно
    confirmationDialogBackground.setPosition(
        (boardActualWidth + UI_AREA_WIDTH) / 2.f - 175.f,
        window.getSize().y / 2.f - 75.f
    );

    confirmationText.setFont(mainFont);
    confirmationText.setCharacterSize(16);
    confirmationText.setFillColor(sf::Color::White);

    // Кнопки для діалогу
    confirmButton = std::make_unique<Button>("Yes", sf::Vector2f(80, 40), 14, mainFont, sf::Color(0, 150, 0),
                                             sf::Color::White);
    cancelButton = std::make_unique<Button>("No", sf::Vector2f(80, 40), 14, mainFont, sf::Color(180, 0, 0),
                                            sf::Color::White);

    propertyOwnerHighlight.setOutlineColor(sf::Color::Transparent); //прямокутник для підсвітки куплених вулиць

    initializeGameUIAndButtons();
    initializePlayers(2);
    initializePlayerStatusUI();
    isRunning = true;
    addSystemMessageToLog("Game started. Welcome to Monopoly!");
}

bool Game::loadResources() {
    if (!boardTexture.loadFromFile("assets/monopoly-2.jpg")) {
        std::cout << "Failed to load background texture!" << std::endl;
        return false;
    }

    if (!mainFont.loadFromFile("assets/fonts/Roboto_Condensed-Regular.ttf")) {
        std::cout << "Failed to load font!" << std::endl;
        return false;
    }

    return true;
}

sf::FloatRect Game::getTileBounds(int tileIndex) const {
    const BoardTile *tile = gameBoard.getTileAt(tileIndex);
    if (!tile) { return {0, 0, 0, 0,}; }

    float tileWidth, tileHeight;
    bool isHorizontal = (tileIndex > 0 && tileIndex < 10) || (tileIndex > 20 && tileIndex < 30);
    bool isVertical = (tileIndex > 10 && tileIndex < 20) || (tileIndex > 30 && tileIndex < 40);

    if (isHorizontal) {
        constexpr float HORIZONTAL_H = 120.f;
        constexpr float HORIZONTAL_W = 72.f;
        tileWidth = HORIZONTAL_W;
        tileHeight = HORIZONTAL_H;
    } else if (isVertical) {
        constexpr float VERTICAL_H = 72.f;
        constexpr float VERTICAL_W = 120.f;
        tileWidth = VERTICAL_W;
        tileHeight = VERTICAL_H;
    } else {
        constexpr float CORNER_SIZE = 120.f;
        tileWidth = CORNER_SIZE;
        tileHeight = CORNER_SIZE;
    }

    sf::Vector2f tilePos = tile->getVisualPosition();
    return {tilePos.x - tileWidth / 2.f, tilePos.y - tileHeight / 2.f, tileWidth, tileHeight};
}

void Game::initializeGameUIAndButtons() {
    chatBackground.setSize(sf::Vector2f(Game::UI_AREA_WIDTH - 2 * Game::PADDING, Game::CHAT_AREA_HEIGHT));
    chatBackground.setPosition(this->uiPanelX + Game::PADDING, Game::PADDING);
    chatBackground.setFillColor(sf::Color(70, 70, 70, 230));
    chatBackground.setOutlineColor(sf::Color(150, 150, 150));
    chatBackground.setOutlineThickness(1.f);

    chatView.setSize(chatBackground.getSize());
    chatView.setCenter(chatBackground.getSize().x / 2.f, chatBackground.getSize().y / 2.f);
    chatView.setViewport(sf::FloatRect(
        chatBackground.getPosition().x / static_cast<float>(window.getSize().x),
        chatBackground.getPosition().y / static_cast<float>(window.getSize().y),
        chatBackground.getSize().x / static_cast<float>(window.getSize().x),
        chatBackground.getSize().y / static_cast<float>(window.getSize().y)
    ));

    chatScrollOffset = 0.f;
    totalLogTextHeight = PADDING;
}

void Game::initializePlayerStatusUI() {
    float statusPanelY = chatBackground.getPosition().y + chatBackground.getSize().y + PADDING;
    playerStatusBackground.setSize({UI_AREA_WIDTH - 2 * PADDING, PLAYER_STATUS_AREA_HEIGHT});
    playerStatusBackground.setPosition(uiPanelX + PADDING, statusPanelY);
    playerStatusBackground.setFillColor(sf::Color(70, 70, 70, 230));
    playerStatusBackground.setOutlineColor(sf::Color(150, 150, 150));
    playerStatusBackground.setOutlineThickness(1.f);

    playerStatusUIs.resize(players.size());
    float entryHeight = PLAYER_STATUS_AREA_HEIGHT / players.size();

    for (size_t i = 0; i < players.size(); ++i) {
        float currentY = statusPanelY + (i * entryHeight) + (entryHeight / 2) - (logCharacterSize / 2.f);
        float startX = uiPanelX + PADDING * 2;

        // Індикатор ходу
        playerStatusUIs[i].turnIndicator.setRadius(5.f);
        playerStatusUIs[i].turnIndicator.setOrigin(5.f, 5.f);
        playerStatusUIs[i].turnIndicator.setPosition(startX, currentY + 5.f);

        // Name player
        playerStatusUIs[i].nameText.setFont(mainFont);
        playerStatusUIs[i].nameText.setCharacterSize(logCharacterSize);
        playerStatusUIs[i].nameText.setFillColor(players[i].getToken().getFillColor());
        playerStatusUIs[i].nameText.setPosition(startX + 15.f, currentY);

        // Money
        playerStatusUIs[i].moneyText.setFont(mainFont);
        playerStatusUIs[i].moneyText.setCharacterSize(logCharacterSize);
        playerStatusUIs[i].moneyText.setFillColor(sf::Color::White);
        playerStatusUIs[i].moneyText.setPosition(startX + 100.f, currentY);

        // Card Jail
        playerStatusUIs[i].jailCardText.setFont(mainFont);
        playerStatusUIs[i].jailCardText.setCharacterSize(logCharacterSize);
        playerStatusUIs[i].jailCardText.setFillColor(sf::Color(255, 215, 0));
        playerStatusUIs[i].jailCardText.setPosition(startX + 180.f, currentY);
    }
}
void Game::updateScrollAfterNewMessage() {
    const float lineHeight = static_cast<float>(logCharacterSize) + 6.f;

    // костиль який працює, пізніше виправлю(завдяки ньому повідомлення в чаті появляються знизу)
    totalLogTextHeight = (static_cast<float>(logEntries.size()) * lineHeight) + 250.f;
    // костиль

    float viewHeight = chatView.getSize().y;


    if (totalLogTextHeight > viewHeight) {
        chatScrollOffset = totalLogTextHeight - viewHeight;
    } else {
        chatScrollOffset = 0.f;
    }
    if (chatScrollOffset < 0.f) chatScrollOffset = 0.f;
}

void Game::addPlayerActionToLog(const Player &player, const std::string &actionMessagePart) {
    LogEntry entry;
    entry.isTwoPartMessage = true;

    // нікнейм з кольором
    entry.part1.setFont(mainFont);
    entry.part1.setString(player.getName() + ": ");
    entry.part1.setCharacterSize(logCharacterSize);
    entry.part1.setFillColor(player.getToken().getFillColor());

    // текст дії білим кольором
    entry.part2.setFont(mainFont);
    entry.part2.setString(actionMessagePart);
    entry.part2.setCharacterSize(logCharacterSize);
    entry.part2.setFillColor(sf::Color::White);

    logEntries.push_back(entry);
    updateScrollAfterNewMessage();
}

// прості системні повідомлення
void Game::addSystemMessageToLog(const std::string &message) {
    LogEntry entry;
    entry.isTwoPartMessage = false;

    entry.part1.setFont(mainFont);
    entry.part1.setString(message);
    entry.part1.setCharacterSize(logCharacterSize);
    entry.part1.setFillColor(sf::Color::White);

    logEntries.push_back(entry);

    updateScrollAfterNewMessage();
}

void Game::handleChatScroll(float delta) {
    if (logEntries.empty() || totalLogTextHeight <= chatView.getSize().y) {
        return;
    }

    constexpr float srollSpeed = 30.f;
    chatScrollOffset -= delta * srollSpeed;

    if (chatScrollOffset < 0.f) {
        chatScrollOffset = 0.f;
    }

    float maxScrollOffset = totalLogTextHeight - chatView.getSize().y;

    if (maxScrollOffset < 0.f) maxScrollOffset = 0.f;
    if (chatScrollOffset > maxScrollOffset) {
        chatScrollOffset = maxScrollOffset;
    }
}

void Game::initializePlayers(int numberOfPlayers) {
    if (numberOfPlayers <= 0) return;
    players.reserve(numberOfPlayers);
    std::vector playerColors = {sf::Color::Red, sf::Color::Blue, sf::Color::Green, sf::Color::Yellow};

    for (int i = 0; i < numberOfPlayers; ++i) {
        std::string playerName = "Player " + std::to_string(i + 1);
        sf::Color playerColor = (i < playerColors.size()) ? playerColors[i] : sf::Color::Black;
        // Колір за замовчуванням

        players.emplace_back(playerName, 1500, playerColor);

        if (const BoardTile *goTile = gameBoard.getTileAt(gameBoard.getGoPosition())) {
            sf::Vector2f goTileScreenPos = goTile->getVisualPosition();
            players.back().setTokenVisualPosition(goTileScreenPos);
        } else {
            std::cerr << "CRITICAL Error: GO tile not found! Cannot position " << playerName << std::endl;
            players.back().setTokenVisualPosition(sf::Vector2f(boardSprite.getPosition().x + 40.f, 50.f));
        }
    }
    currentPlayerIndex = 0;
    if (!players.empty()) {
        addSystemMessageToLog("Initialized " + std::to_string(players.size()) + " players");
        addPlayerActionToLog(players[currentPlayerIndex], "starts the game");
    }
}

void Game::updateTokenPositionsOnTile(int tileIndex) {
    if (tileIndex < 0 || static_cast<size_t>(tileIndex) >= GameBoard::getTotalSquares()) {
        return;
    }

    const BoardTile *tile = gameBoard.getTileAt(tileIndex);
    if (!tile) return;

    sf::Vector2f centerOfTile = tile->getVisualPosition();

    std::vector<Player *> playersOnThisTile;
    for (Player &p: players) {
        if (p.getCurrentPosition() == tileIndex && !p.getIsBankrupt()) {
            playersOnThisTile.push_back(&p);
        }
    }

    int numPlayersOnTile = playersOnThisTile.size();
    if (numPlayersOnTile == 0) {
        return;
    }

    constexpr float baseOffset = 20.f;
    std::vector<sf::Vector2f> offsets;

    if (numPlayersOnTile == 1) {
        offsets.emplace_back(0.f, 0.f);
    } else if (numPlayersOnTile == 2) {
        offsets.emplace_back(-baseOffset, 0.f); // Перший ліворуч
        offsets.emplace_back(baseOffset, 0.f); // Другий праворуч
    } else if (numPlayersOnTile == 3) {
        offsets.emplace_back(0.f, -baseOffset); // Вгорі
        offsets.emplace_back(-baseOffset, baseOffset / 2.f); // Внизу зліва
        offsets.emplace_back(baseOffset, baseOffset / 2.f); // Внизу справа
    } else if (numPlayersOnTile >= 4) {
        offsets.emplace_back(-baseOffset, -baseOffset); // Верхній лівий
        offsets.emplace_back(baseOffset, -baseOffset); // Верхній правий
        offsets.emplace_back(-baseOffset, baseOffset); // Нижній лівий
        offsets.emplace_back(baseOffset, baseOffset); // Нижній правий
    }

    for (size_t i = 0; i < playersOnThisTile.size(); ++i) {
        if (i < offsets.size()) {
            playersOnThisTile[i]->setTokenVisualPosition(centerOfTile + offsets[i]);
        } else {
            playersOnThisTile[i]->setTokenVisualPosition(
                centerOfTile + sf::Vector2f(static_cast<float>(i - 3) * 3.f, static_cast<float>(i - 3) * 3.f));
            // Невеликий діагональний зсув для решти
        }
    }
}

void Game::enterMortgageSelectionMode() {
    if (players.empty()) return;

    Player &currentPlayer = players[currentPlayerIndex];
    addPlayerActionToLog(currentPlayer, "want to mortgage a property.");

    bool canMortgageSomething = false;
    for (Street *street: currentPlayer.getOwnedStreets()) {
        if (street && !street->getIsMortgaged() && street->getHousesBuilt() == 0) {
            canMortgageSomething = true;
            break;
        }
    }

    if (!canMortgageSomething) {
        addSystemMessageToLog("You have no properties avaliable to mortgage.");
        return;
    }

    currentGameState = GameState::AWAITING_PROPERTY_SELECTION_FOR_MORTGAGE;
    addSystemMessageToLog("Click on a property to a mortgage. (Press Esc to cancel)");
}

void Game::enterUnmortgageSelectionMode() {
    if (players.empty()) return;

    const Player &currentPlayer = players[currentPlayerIndex];
    addPlayerActionToLog(currentPlayer, "want to unmortgage a property.");

    bool canUnmortgageSomething = false;
    for (const Street *street: currentPlayer.getOwnedStreets()) {
        if (street && street->getIsMortgaged()) {
            canUnmortgageSomething = true;
            break;
        }
    }
    if (!canUnmortgageSomething) {
        addSystemMessageToLog("You have no mortgaged properties to redeem");
        return;
    }

    currentGameState = GameState::AWAITING_PROPERTY_SELECTION_FOR_UNMORTGAGE;
    addSystemMessageToLog("Click on a mortgaged property to redeem if. (Press Esc to cancel)");
}

void Game::enterBuildSelectionMode() {
    if (players.empty()) return;
    const Player &currentPlayer = players[currentPlayerIndex];
    addPlayerActionToLog(currentPlayer, "want to build a house.");

    bool canBuildSomething = false;
    for (const Street *street: currentPlayer.getOwnedStreets()) {
        if (currentPlayer.canBuildOnStreet(street, groupsBuiltOnThisTurn)) {
            if (currentPlayer.getMoney() >= street->getHouseCost()) {
                canBuildSomething = true;
                break;
            }
        }
    }

    if (!canBuildSomething) {
        addSystemMessageToLog("You don't have all street.");
        return;
    }

    currentGameState = GameState::AWAITING_PROPERTY_SELECTION_FOR_BUILDING;
    addSystemMessageToLog("Click on a property to build a house.(Press Esc to cancel)");
}

void Game::enterSellHouseSelectionMode() {
    if (players.empty()) return;
    const Player &currentPlayer = players[currentPlayerIndex];
    addPlayerActionToLog(currentPlayer, "want sell House");

    bool canSellSomething = false;
    for (const Street *street: currentPlayer.getOwnedStreets()) {
        if (currentPlayer.canSellHouseOnStreet(street)) {
            canSellSomething = true;
            break;
        }
    }

    if (!canSellSomething) {
        addSystemMessageToLog("You don't have House for sell.");
        return;
    }

    currentGameState = GameState::AWAITING_PROPERTY_SELECTION_FOR_SELL_HOUSE;
    addSystemMessageToLog("Click on a property to select a house.(Press Esc To Cancel)");
}

void Game::handlePropertySelectionForMortgage(sf::Vector2i mousePosition) {
    if (players.empty()) return;

    Player &currentPlayer = players[currentPlayerIndex];
    for (Street *street: currentPlayer.getOwnedStreets()) {
        if (street && !street->getIsMortgaged() && street->getHousesBuilt() == 0) {
            int tileIndex = gameBoard.findTileIndexByStreet(street);
            sf::FloatRect tileBounds = getTileBounds(tileIndex);

            if (tileBounds.contains(static_cast<sf::Vector2f>(mousePosition))) {
                selectedStreet = street;
                int mortgageValue = selectedStreet->getPurchasePrice() / 2;
                std::string msg = "Mortgage " + selectedStreet->getName() + "\nfor $" + std::to_string(mortgageValue) +
                                  "?";
                showConfirmationDialog(msg, GameButtonType::MORTGAGE, GameButtonType::NONE);
                return;
            }
        }
    }
}

void Game::handlePropertySelectionForUnmortgage(sf::Vector2i mousePosition) {
    if (players.empty()) return;

    Player &currentPlayer = players[currentPlayerIndex];
    for (Street *street: currentPlayer.getOwnedStreets()) {
        if (street && street->getIsMortgaged()) {
            const int tileIndex = gameBoard.findTileIndexByStreet(street);
            sf::FloatRect tileBounds = getTileBounds(tileIndex);

            if (tileBounds.contains(static_cast<sf::Vector2f>(mousePosition))) {
                selectedStreet = street;
                const int redeemCost = static_cast<int>((street->getPurchasePrice() / 2) * 1.10);

                if (currentPlayer.getMoney() < redeemCost) {
                    addSystemMessageToLog("You don't have enough money to redeem " + street->getName() +
                                          " (costs $" + std::to_string(redeemCost) + ").");
                    selectedStreet = nullptr;
                    return;
                }

                std::string msg = "Redeem " + selectedStreet->getName() + "\nfor $" + std::to_string(redeemCost) + "?";
                showConfirmationDialog(msg, GameButtonType::UNMORTGAGE, GameButtonType::NONE);
                // Використовуємо UNMORTGAGE як ідентифікатор
                return;
            }
        }
    }
}

void Game::handlePropertySelectionForBuilding(sf::Vector2i mousePosition) {
    if (players.empty()) return;
    Player &currentPlayer = players[currentPlayerIndex];

    for (Street *street: currentPlayer.getOwnedStreets()) {
        if (currentPlayer.canBuildOnStreet(street, groupsBuiltOnThisTurn)) {
            const int tileIndex = gameBoard.findTileIndexByStreet(street);
            sf::FloatRect tileBounds = getTileBounds(tileIndex);

            if (tileBounds.contains(static_cast<sf::Vector2f>(mousePosition))) {
                selectedStreet = street;
                int houseCost = selectedStreet->getHouseCost();
                if (currentPlayer.getMoney() < houseCost) {
                    addSystemMessageToLog(
                        "You don't have enough money to build here (costs $" + std::to_string(houseCost) + ").");
                    selectedStreet = nullptr;
                    return;
                }
                std::string houseOrHotel = (street->getHousesBuilt() == 4) ? "a hotel" : "a house";
                std::string msg = "Build " + houseOrHotel + " on " + street->getName() + "\nfor $" +
                                  std::to_string(houseCost) + "?";
                showConfirmationDialog(msg, GameButtonType::BUILD_HOUSE, GameButtonType::NONE);
                return;
            }
        }
    }
}

void Game::handlePropertySelectionForSellHouse(sf::Vector2i mousePosition) {
    if (players.empty()) return;
    Player &currentPlayer = players[currentPlayerIndex];
    for (Street *street: currentPlayer.getOwnedStreets()) {
        if (currentPlayer.canSellHouseOnStreet(street)) {
            const int tileIndex = gameBoard.findTileIndexByStreet(street);
            sf::FloatRect tileBounds = getTileBounds(tileIndex);
            if (tileBounds.contains(static_cast<sf::Vector2f>(mousePosition))) {
                selectedStreet = street;
                int sellPrice = selectedStreet->getHouseCost() / 2;
                std::string objectToSell = (selectedStreet->getHousesBuilt() == 5) ? "a hotel" : "a house";
                std::string msg = "Sell " + objectToSell + " on " + selectedStreet->getName() + "\nfor $" +
                                  std::to_string(sellPrice) + "?";
                showConfirmationDialog(msg, GameButtonType::SELL_HOUSE, GameButtonType::NONE);
                return;
            }
        }
    }
}

void Game::showConfirmationDialog(const std::string &message, GameButtonType confirmAction,
                                  GameButtonType cancelAction) {
    stateBeforeConfirmation = currentGameState;
    currentGameState = GameState::CONFIRMATION_DIALOG;

    confirmationText.setString(message);
    sf::FloatRect textRect = confirmationText.getLocalBounds();
    confirmationText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    confirmationText.setPosition(
        confirmationDialogBackground.getPosition().x + confirmationDialogBackground.getSize().x / 2.0f,
        confirmationDialogBackground.getPosition().y + 40.f
    );

    // Розміщуємо кнопки
    float dialogX = confirmationDialogBackground.getPosition().x;
    float dialogY = confirmationDialogBackground.getPosition().y;
    float dialogW = confirmationDialogBackground.getSize().x;
    float buttonW = confirmButton->getGlobalBounds().width;

    confirmButton->setPosition({dialogX + 40, dialogY + 80});
    cancelButton->setPosition({dialogX + dialogW - 40 - buttonW, dialogY + 80});
}

void Game::hideConfirmationDialog() {
    currentGameState = stateBeforeConfirmation;
    if (currentGameState != GameState::NORMAL_TURN) {
        currentGameState = GameState::NORMAL_TURN;
    }
    selectedStreet = nullptr;
}

void Game::handleButtonAction(GameButtonType actionType) {
    if (players.empty() || !isRunning) return;

    Player &currentPlayerRef = players[currentPlayerIndex];
    std::vector<Player *> allPlayersPtrs;
    for (Player &p: players) { allPlayersPtrs.push_back(&p); }

    // Перед виконанням дії кнопки, яка може змінити стан (наприклад, ROLL),
    // варто перевірити, чи ця дія дозволена (аналогічно до updateButtonStates).
    // Наприклад, для ROLL: if (rollDoneThisTurn || (doublesRolledCount >=3 && currentPlayerRef.getIsInJail())) return;

    switch (actionType) {
        case GameButtonType::ROLL: {
            if (this->rollDoneThisTurn) {
                addPlayerActionToLog(currentPlayerRef,
                                     "has already completed their roll action for this part of the turn.");
                break;
            }

            playerJustLanded = false; // для того, щоб не можна було купити вулицю на початку ходу

            const bool playerWasInJailAtStartOfRoll = currentPlayerRef.getIsInJail();
            const int die1 = diceDistribution(randomEngine);
            const int die2 = diceDistribution(randomEngine);
            const int diceRoll = die1 + die2;
            const bool isDouble = (die1 == die2);

            if (!isDouble) {
                doublesRolledCount = 0;
            }

            const std::string rollMsg = "rolled " + std::to_string(die1) + " + " + std::to_string(die2) +
                                        " = " + std::to_string(diceRoll) + (isDouble ? " (DOUBLES!)" : "");
            addPlayerActionToLog(currentPlayerRef, rollMsg);


            if (playerWasInJailAtStartOfRoll) {
                if (isDouble) {
                    addPlayerActionToLog(currentPlayerRef, "rolled doubles and is FREE FROM JAIL!");
                    currentPlayerRef.leaveJail();
                    doublesRolledCount = 0; // Скидаємо, бо вийшов
                    // Гравець рухається, але ми не виходимо звідси, потік піде далі
                } else {
                    // Не дубль у в'язниці
                    currentPlayerRef.incrementTurnsInJail();
                    addPlayerActionToLog(currentPlayerRef, "failed to roll doubles in jail.");
                    rollDoneThisTurn = true; // Спроба зроблена
                    handleButtonAction(GameButtonType::END_TURN);
                    break; // Хід закінчується, гравець не рухається
                }
            }

            // 5. Єдиний блок для переміщення та дій на клітинці
            // Цей блок виконується, якщо гравець був на волі АБО щойно вийшов з в'язниці по дублю.
            bool passedGo = false;
            int oldPos = currentPlayerRef.getCurrentPosition();
            int newPosition = gameBoard.movePlayer(currentPlayerRef, diceRoll, passedGo);
            if (oldPos != newPosition) {
                updateTokenPositionsOnTile(oldPos);
            }
            updateTokenPositionsOnTile(newPosition);

            if (passedGo) {
                addPlayerActionToLog(currentPlayerRef, "collected $200 for passing GO!");
                currentPlayerRef.addMoney(200);
            }

            // Викликаємо логіку клітинки ОДИН раз і перевіряємо результат
            gameBoard.playerLandedOnSquare(currentPlayerRef, newPosition, allPlayersPtrs);
            if (currentPlayerRef.getIsInJail() && !playerWasInJailAtStartOfRoll) {
                addSystemMessageToLog(currentPlayerRef.getName() + "'s turn ends as they were sent to jail.");
                handleButtonAction(GameButtonType::END_TURN);
                break;
            }

            // Якщо хід не був завершений, встановлюємо прапорці
            playerJustLanded = true;
            rollDoneThisTurn = true;

            // 6. Обробка дублів (якщо гравець не починав хід у в'язниці)
            if (isDouble && !playerWasInJailAtStartOfRoll) {
                this->doublesRolledCount++;
                addPlayerActionToLog(currentPlayerRef,
                                     "gets another turn! (Doubles count: " + std::to_string(this->doublesRolledCount) +
                                     ")");
                if (this->doublesRolledCount == 3) {
                    addPlayerActionToLog(currentPlayerRef, "rolled 3 doubles in a row and GOES TO JAIL!");
                    int old_pos = currentPlayerRef.getCurrentPosition();
                    currentPlayerRef.sendToJail(gameBoard.getJailPosition());
                    updateTokenPositionsOnTile(old_pos);
                    updateTokenPositionsOnTile(currentPlayerRef.getCurrentPosition());
                    // Хід автоматично передається
                    handleButtonAction(GameButtonType::END_TURN);
                } else {
                    // Дозволяємо ще один кидок
                    this->rollDoneThisTurn = false;
                }
            }
            break;
        }

        case GameButtonType::BUY: {
            const BoardTile *currentTile = gameBoard.getTileAt(currentPlayerRef.getCurrentPosition());

            if (!currentTile || currentTile->getType() != BoardTile::TileType::PROPERTY) {
                addPlayerActionToLog(currentPlayerRef, "tries to buy, but there is nothing to buy here.");
                break; // Виходимо, якщо це не клітинка власності
            }

            auto *streetTile = dynamic_cast<StreetTile *>(const_cast<BoardTile *>(currentTile));
            Street &propertyToBuy = streetTile->getPropertyData();

            if (propertyToBuy.getOwner() == nullptr && currentPlayerRef.getMoney() >= propertyToBuy.
                getPurchasePrice()) {
                // Виконуємо покупку
                if (propertyToBuy.buyStreet(&currentPlayerRef)) {
                    addPlayerActionToLog(currentPlayerRef, "bought " + propertyToBuy.getName() +
                                                           " for $" + std::to_string(propertyToBuy.getPurchasePrice()));
                    playerJustLanded = false;
                } else {
                    // Цей випадок малоймовірний, якщо попередні перевірки пройшли, але корисний для відлагодження
                    addSystemMessageToLog("Error: Purchase of " + propertyToBuy.getName() + " failed unexpectedly.");
                }
            } else if (propertyToBuy.getOwner() != nullptr) {
                addPlayerActionToLog(currentPlayerRef,
                                     "cannot buy " + propertyToBuy.getName() + ", it's already owned by " +
                                     propertyToBuy.getOwner()->getName() + ".");
            } else {
                addPlayerActionToLog(currentPlayerRef, "cannot afford to buy " + propertyToBuy.getName() + ".");
            }
            // Після покупки, наступний виклик Game::update() автоматично деактивує кнопку "BUY",
            // оскільки propertyToBuy.getOwner() вже не буде nullptr. Ніяких додаткових дій не потрібно.
            break;
        }

        case GameButtonType::SELL_HOUSE: {
            enterSellHouseSelectionMode();
            break;
        }

        case GameButtonType::MORTGAGE: {
            enterMortgageSelectionMode();
            break;
        }
        case GameButtonType::UNMORTGAGE: {
            enterUnmortgageSelectionMode();
            break;
        }
        case GameButtonType::BUILD_HOUSE: {
            enterBuildSelectionMode();
            break;
        }
        case GameButtonType::PAY_OR_USE_CARD_JAIL: {
            if (!currentPlayerRef.getIsInJail()) {
                addSystemMessageToLog("You are not in jail.");
                break;
            }

            if (currentPlayerRef.useGetOutOfJailFreeCard()) {
                addPlayerActionToLog(currentPlayerRef, "used a 'Get Out Of Jail Free' card");
                currentPlayerRef.leaveJail();
            } else if (currentPlayerRef.getMoney() >= 50) {
                if (currentPlayerRef.subtractMoney(50)) {
                    addPlayerActionToLog(currentPlayerRef, "paid $50 to get out of jail.");
                    currentPlayerRef.leaveJail();
                }
            } else {
                addSystemMessageToLog("You have neither a card nor enough money($50) to get out.");
                rollDoneThisTurn = true;
            }
            break;
        }

        case GameButtonType::END_TURN: {
            if (rollDoneThisTurn) {
                addPlayerActionToLog(currentPlayerRef, "ends their turn.");
                currentPlayerIndex = (currentPlayerIndex + 1) % static_cast<int>(players.size());
                rollDoneThisTurn = false;
                doublesRolledCount = 0;
                playerJustLanded = false;
                groupsBuiltOnThisTurn.clear();
                if (!players.empty()) {
                    addSystemMessageToLog("--- Next turn ---");
                    addPlayerActionToLog(players[currentPlayerIndex], "it's your turn.");
                }
            } else {
                currentPlayerIndex = (currentPlayerIndex + 1) % static_cast<int>(players.size());
                rollDoneThisTurn = false;
                doublesRolledCount = 0;
                playerJustLanded = false;
                groupsBuiltOnThisTurn.clear();
                if (!players.empty()) {
                    addSystemMessageToLog("--- Next turn ---");
                    addPlayerActionToLog(players[currentPlayerIndex], "it's your turn.");
                }
            }
            break;
        }
        case GameButtonType::NONE: {
        default:
            break;
        }
    }
}

void Game::processEvents() {
    sf::Event event{};
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
            isRunning = false;
        }
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
            if (currentGameState == GameState::AWAITING_PROPERTY_SELECTION_FOR_MORTGAGE ||
                currentGameState == GameState::AWAITING_PROPERTY_SELECTION_FOR_UNMORTGAGE ||
                currentGameState == GameState::AWAITING_PROPERTY_SELECTION_FOR_BUILDING ||
                currentGameState == GameState::AWAITING_PROPERTY_SELECTION_FOR_SELL_HOUSE) {
                currentGameState = GameState::NORMAL_TURN;
                selectedStreet = nullptr;
                addSystemMessageToLog("Selection cancelled.");
            } else if (currentGameState == GameState::CONFIRMATION_DIALOG) {
                hideConfirmationDialog();
            } else {
                window.close();
                isRunning = false;
            }
        }
        if (event.type == sf::Event::MouseWheelScrolled) {
            if (uiManager) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                if (chatBackground.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                    handleChatScroll(event.mouseWheelScroll.delta);
                }
            }
        }

        // Обробка кліків по кнопках
        if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);

            switch (currentGameState) {
                case GameState::NORMAL_TURN:
                    if (uiManager) {
                        GameButtonType clicked = uiManager->handleMouseClick(mousePos, window);
                        if (clicked != GameButtonType::NONE) handleButtonAction(clicked);
                    }
                    break;
                case GameState::AWAITING_PROPERTY_SELECTION_FOR_MORTGAGE:
                    handlePropertySelectionForMortgage(mousePos);
                    break;
                case GameState::AWAITING_PROPERTY_SELECTION_FOR_UNMORTGAGE:
                    handlePropertySelectionForUnmortgage(mousePos);
                    break;
                case GameState::AWAITING_PROPERTY_SELECTION_FOR_BUILDING:
                    handlePropertySelectionForBuilding(mousePos);
                    break;
                case GameState::AWAITING_PROPERTY_SELECTION_FOR_SELL_HOUSE:
                    handlePropertySelectionForSellHouse(mousePos);
                    break;
                case GameState::CONFIRMATION_DIALOG:
                    // Обробка кліків по кнопках діалогу
                    if (confirmButton->isMouseOver(window)) {
                        // Дія підтверджена
                        if (!selectedStreet) {
                            hideConfirmationDialog();
                            return;
                        }

                        Street &street = *selectedStreet;
                        Player &currentPlayer = players[currentPlayerIndex];

                        if (stateBeforeConfirmation == GameState::AWAITING_PROPERTY_SELECTION_FOR_MORTGAGE) {
                            if (street.mortgageStreet(&currentPlayer)) {
                                addPlayerActionToLog(currentPlayer,
                                                     "mortgaged " + street.getName() + " for $" + std::to_string(
                                                         street.getPurchasePrice() / 2));
                            }
                        } else if (stateBeforeConfirmation == GameState::AWAITING_PROPERTY_SELECTION_FOR_UNMORTGAGE) {
                            int redeemCost = static_cast<int>((street.getPurchasePrice() / 2) * 1.10);
                            if (street.redeemMortgage(&currentPlayer)) {
                                addPlayerActionToLog(currentPlayer,
                                                     "redeemed " + street.getName() + " for $" + std::to_string(
                                                         redeemCost));
                            } else {
                                addPlayerActionToLog(currentPlayer,
                                                     "failed to redeem " + street.getName() + ". Not enough money?");
                            }
                        } else if (stateBeforeConfirmation == GameState::AWAITING_PROPERTY_SELECTION_FOR_BUILDING) {
                            if (street.buildHouse(&currentPlayer)) {
                                std::string houseOrHotel = (street.getHousesBuilt() == 5)
                                                               ? "a hotel"
                                                               : "house #" + std::to_string(street.getHousesBuilt());
                                addPlayerActionToLog(currentPlayer,
                                                     "built " + houseOrHotel + " on " + street.getName());
                                groupsBuiltOnThisTurn.insert(street.getGroup());
                            } else {
                                addPlayerActionToLog(currentPlayer, "failed to build on " + street.getName());
                            }
                        } else if (stateBeforeConfirmation == GameState::AWAITING_PROPERTY_SELECTION_FOR_SELL_HOUSE) {
                            int moneyGained = street.sellHouse();
                            if (moneyGained > 0) {
                                currentPlayer.addMoney(moneyGained);
                                std::string objectSold = (street.getHousesBuilt() + 1 == 5) ? "hotel" : "house";
                                addPlayerActionToLog(currentPlayer, "sold a " + objectSold + " on " +
                                                                    street.getName() + " for $" + std::to_string(
                                                                        moneyGained));
                            }
                        }
                        hideConfirmationDialog();
                    } else if (cancelButton->isMouseOver(window)) {
                        hideConfirmationDialog();
                    }
                    break;
            }
        }


        if (event.type == sf::Event::MouseMoved) {
            if (currentGameState == GameState::CONFIRMATION_DIALOG) {
            } else if (uiManager) {
                uiManager->handleMouseHover(sf::Mouse::getPosition(window), window);
            }
        }
    }
}

void Game::update(sf::Time deltaTime) {
    if (uiManager && !players.empty()) {
        Player &currentPlayer = players[currentPlayerIndex];
        bool gameIsInJail = currentPlayer.getIsInJail(); // перевірка чи гравець у вязниці

        if (gameIsInJail && rollDoneThisTurn && currentPlayer.getTurnsInJail() == 0) {
            if (doublesRolledCount < 3) {
                addSystemMessageToLog(currentPlayer.getName() + " was sent to jail! Turn ends.");
                // Викликаємо логіку завершення ходу
                handleButtonAction(GameButtonType::END_TURN);
                return;
            }
        }

        bool canRoll = false;
        if (!rollDoneThisTurn) { canRoll = true; }
        if (currentPlayer.getTurnsInJail() >= 3) { canRoll = false; }

        // BUY
        bool canBuy = false; // Поки що false, потрібна логіка перевірки клітинки
        if (playerJustLanded) {
            const BoardTile *currentTile = gameBoard.getTileAt(currentPlayer.getCurrentPosition());
            if (currentTile && currentTile->getType() == BoardTile::TileType::PROPERTY) {
                const auto *streetTile = dynamic_cast<const StreetTile *>(currentTile);
                if (streetTile->getPropertyData().getOwner() == nullptr &&
                    currentPlayer.getMoney() >= streetTile->getPropertyData().getPurchasePrice()) {
                    // Купувати можна тільки ПІСЛЯ кидка
                    canBuy = true;
                }
            }
        }


        // END TURN
        bool canEndTurn = false;
        bool mustRollAgain = (doublesRolledCount > 0 && doublesRolledCount < 3 && !gameIsInJail);
        if (rollDoneThisTurn && !mustRollAgain) {
            canEndTurn = true;
        }

        // PAY/USE CARD
        bool canPayOrUseCardForJail = false;
        if (gameIsInJail) {
            if (currentPlayer.getMoney() >= 50 || currentPlayer.getGetOutOfJailFreeCardsCount() > 0) {
                canPayOrUseCardForJail = true;
            }
        }
        // MANAGE PROPERTIES (загальний прапорець, якщо є власність)
        const bool canManageProperties = !currentPlayer.getOwnedStreets().empty();
        bool canMortgage = false;
        bool canUnmortgage = false;
        bool canBuild = false;
        bool canSellHouse = false;

        if (canManageProperties) {
            // Окремий цикл для MORTGAGE
            for (const auto *street: currentPlayer.getOwnedStreets()) {
                if (!street->getIsMortgaged() && street->getHousesBuilt() == 0) {
                    canMortgage = true;
                    break;
                }
            }
            // Окремий цикл для UNMORTGAGE
            for (const auto *street: currentPlayer.getOwnedStreets()) {
                if (street->getIsMortgaged() && currentPlayer.getMoney() >= static_cast<int>(
                        (street->getPurchasePrice() / 2) * 1.10)) {
                    canUnmortgage = true;
                    break;
                }
            }
            // Окремий цикл для BUILD
            for (const auto *street: currentPlayer.getOwnedStreets()) {
                if (!rollDoneThisTurn) {
                    if (currentPlayer.canBuildOnStreet(street, groupsBuiltOnThisTurn) && currentPlayer.getMoney() >=
                        street->getHouseCost()) {
                        canBuild = true;
                        break;
                    }
                }
            }
            // Окремий цикл для SELL HOUSE
            for (const auto *street: currentPlayer.getOwnedStreets()) {
                if (currentPlayer.canSellHouseOnStreet(street)) {
                    canSellHouse = true;
                    break;
                }
            }
        }

        uiManager->updateButtonsStates( currentPlayer, canRoll, canBuy, canMortgage, canUnmortgage, canBuild, canSellHouse,
                                        canPayOrUseCardForJail, canManageProperties, canEndTurn, gameIsInJail);

        // Оновлення панелі гравців
        for (size_t i = 0; i < players.size(); ++i) {
            playerStatusUIs[i].nameText.setString(players[i].getName());
            playerStatusUIs[i].moneyText.setString("$" + std::to_string(players[i].getMoney()));
            playerStatusUIs[i].jailCardText.setString("J-card: " + std::to_string(players[i].getGetOutOfJailFreeCardsCount()));

            if (i== static_cast<size_t>(currentPlayerIndex)) {
                playerStatusUIs[i].turnIndicator.setFillColor(sf::Color::Yellow);
            } else {
                playerStatusUIs[i].turnIndicator.setFillColor(sf::Color::Transparent);
            }
        }
    }
}

void Game::render() {
    window.clear(sf::Color(50,50,50));
    window.draw(boardSprite);

    renderOwnedProperties();  // підсвічування куплених вулиць

    renderSelectionHighlights();  //підсвічення вулиць коли вибираємо закласти чи викупити

    for (const auto& player : players) {
        player.drawToken(window);
    }

    window.draw(chatBackground);

    // Draw Player Panel
    window.draw(playerStatusBackground);
    for (const auto& statusUI : playerStatusUIs) {
        window.draw(statusUI.turnIndicator);
        window.draw(statusUI.nameText);
        window.draw(statusUI.moneyText);
        window.draw(statusUI.jailCardText);

    }

    if (uiManager) {
        uiManager->draw(window);
    }

    sf::View currentMainView  = window.getView();
    sf::Vector2f viewSize = chatView.getSize();
    chatView.setCenter(viewSize.x / 2.f, viewSize.y / 2.f + chatScrollOffset);
    window.setView(this->chatView);

    const float lineHeight = static_cast<float>(logCharacterSize) + 6.f;
    constexpr float paddingX_in_view = 5.f;
    float currentTextLine_Y_in_view = totalLogTextHeight - paddingX_in_view;

    for (auto entryIt = logEntries.rbegin(); entryIt != logEntries.rend(); ++entryIt) {
        LogEntry &entry = *entryIt;
        float currentSegmentX_in_view = paddingX_in_view;

        sf::FloatRect part1Bounds = entry.part1.getLocalBounds();
        entry.part1.setPosition(currentSegmentX_in_view,
                                currentTextLine_Y_in_view - part1Bounds.height - part1Bounds.top);
        window.draw(entry.part1);

        if (entry.isTwoPartMessage) {
            currentSegmentX_in_view += entry.part1.getGlobalBounds().width;
            sf::FloatRect part2Bounds = entry.part2.getLocalBounds();
            entry.part2.setPosition(currentSegmentX_in_view,
                                    currentTextLine_Y_in_view - part2Bounds.height - part2Bounds.top);
            window.draw(entry.part2);
        }
        currentTextLine_Y_in_view -= lineHeight;
    }

    window.setView(currentMainView);
    renderConfirmationDialog(); // діалогове вікно
    window.display();
}

void Game::renderSelectionHighlights() {
    if (players.empty()) return;
    if (currentGameState != GameState::AWAITING_PROPERTY_SELECTION_FOR_MORTGAGE &&
        currentGameState != GameState::AWAITING_PROPERTY_SELECTION_FOR_UNMORTGAGE &&
        currentGameState != GameState::AWAITING_PROPERTY_SELECTION_FOR_BUILDING &&
        currentGameState != GameState::AWAITING_PROPERTY_SELECTION_FOR_SELL_HOUSE) {
        return;
    }


    const Player &currentPlayer = players[currentPlayerIndex];
    for (const Street *street: currentPlayer.getOwnedStreets()) {
        bool shouldHighlight = false;

        if (currentGameState == GameState::AWAITING_PROPERTY_SELECTION_FOR_MORTGAGE &&
            street && !street->getIsMortgaged() && street->getHousesBuilt() == 0) {
            shouldHighlight = true;
            selectionHighlight.setFillColor(sf::Color(255, 215, 0, 100)); // Золотий для застави
        } else if (currentGameState == GameState::AWAITING_PROPERTY_SELECTION_FOR_UNMORTGAGE &&
                   street && street->getIsMortgaged()) {
            shouldHighlight = true;
            selectionHighlight.setFillColor(sf::Color(0, 150, 255, 100)); // Синій для викупу
        } else if (currentGameState == GameState::AWAITING_PROPERTY_SELECTION_FOR_BUILDING) {
            if (currentPlayer.canBuildOnStreet(street, groupsBuiltOnThisTurn)) {
                shouldHighlight = true;
                selectionHighlight.setFillColor(sf::Color(0, 255, 0, 120));
            }
        } else if (currentGameState == GameState::AWAITING_PROPERTY_SELECTION_FOR_SELL_HOUSE) {
            if (currentPlayer.canSellHouseOnStreet(street)) {
                if (currentPlayer.getMoney() >= street->getHouseCost()) {
                    shouldHighlight = true;
                    selectionHighlight.setFillColor(sf::Color(255, 0, 0, 120));
                }
            }
        }

        if (shouldHighlight) {
            const int tileIndex = gameBoard.findTileIndexByStreet(street);
            const BoardTile *tile = gameBoard.getTileAt(tileIndex);

            if (!tile) continue;

            auto bounds = getTileBounds(tileIndex);

            selectionHighlight.setSize({bounds.width, bounds.height});
            selectionHighlight.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
            selectionHighlight.setPosition(tile->getVisualPosition());
            window.draw(selectionHighlight);
        }
    }
}

void Game::renderConfirmationDialog() {
    if (currentGameState != GameState::CONFIRMATION_DIALOG) return;
    window.draw(confirmationDialogBackground);
    window.draw(confirmationText);
    confirmButton->drawTo(window);
    cancelButton->drawTo(window);
}

void Game::renderOwnedProperties() {
    for (int i = 0; i < GameBoard::getTotalSquares(); ++i) {
        const BoardTile *tile = gameBoard.getTileAt(i);
        if (!tile || tile->getType() != BoardTile::TileType::PROPERTY) {
            continue;
        }

        // --- ВИЗНАЧАЄМО 'bounds' ТУТ, ОДРАЗУ ---
        auto bounds = getTileBounds(i);

        const auto *streetTile = dynamic_cast<const StreetTile *>(tile);
        if (const Player *owner = streetTile->getPropertyData().getOwner()) {
            sf::Color ownerColor = owner->getToken().getFillColor();
            ownerColor.a = 70;

            propertyOwnerHighlight.setFillColor(ownerColor);
            // 'bounds' вже визначено, просто використовуємо
            propertyOwnerHighlight.setSize({bounds.width, bounds.height});
            propertyOwnerHighlight.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
            propertyOwnerHighlight.setPosition(tile->getVisualPosition());
            window.draw(propertyOwnerHighlight);

            int houses = streetTile->getPropertyData().getHousesBuilt();
            if (houses > 0) {
                sf::RectangleShape houseRect;
                houseRect.setOutlineColor(sf::Color::Black);
                houseRect.setOutlineThickness(1.f);

                bool isBottomRow = (i > 0 && i < 10);
                bool isLeftRow = (i > 10 && i < 20);
                bool isTopRow = (i > 20 && i < 30);
                bool isRightRow = (i > 30 && i < 40);

                const float houseSpacing = 18.f;
                const float offsetFromTile = 35.f;

                if (houses == 5) {
                    // Малюємо готель
                    houseRect.setSize({40.f, 20.f});
                    houseRect.setFillColor(sf::Color::Red);
                    houseRect.setOrigin(20.f, 10.f);

                    sf::Vector2f hotelOffset;
                    if (isBottomRow) hotelOffset = {0.f, -offsetFromTile};
                    else if (isTopRow) hotelOffset = {0.f, offsetFromTile};
                    else if (isLeftRow) hotelOffset = {offsetFromTile, 0.f};
                    else if (isRightRow) hotelOffset = {-offsetFromTile, 0.f};

                    if (isLeftRow || isRightRow) {
                        houseRect.setRotation(90.f);
                    } else {
                        houseRect.setRotation(0.f);
                    }
                    houseRect.setPosition(tile->getVisualPosition() + hotelOffset);
                    window.draw(houseRect);
                } else {
                    // Малюємо будинки (1-4)
                    houseRect.setSize({15.f, 15.f});
                    houseRect.setFillColor(sf::Color::Green);
                    houseRect.setOrigin(7.5f, 7.5f);

                    // Якщо клітинка горизонтальна (верхня або нижня)
                    if (isTopRow || isBottomRow) {
                        float totalWidth = (houses - 1) * houseSpacing;
                        float yOffset = isBottomRow ? -offsetFromTile : offsetFromTile;
                        houseRect.setRotation(0.f);

                        for (int j = 0; j < houses; ++j) {
                            float xOffset = -totalWidth / 2.f + j * houseSpacing;
                            houseRect.setPosition(tile->getVisualPosition() + sf::Vector2f(xOffset, yOffset));
                            window.draw(houseRect);
                        }
                    }
                    // Якщо клітинка вертикальна (ліва або права)
                    else if (isLeftRow || isRightRow) {
                        float xSideOffset = isLeftRow ? offsetFromTile : -offsetFromTile;
                        houseRect.setRotation(90.f);

                        // 1. Знаходимо Y-координату верхнього краю клітинки
                        float tileTopY = tile->getVisualPosition().y - bounds.height / 2.f;

                        // 2. Розраховуємо початкову позицію для першого будинку.
                        float startY = tileTopY + houseRect.getSize().y / 2.f + 5.f; // 5.f - відступ

                        for (int j = 0; j < houses; ++j) {
                            // 3. Кожен наступний будинок малюємо нижче попереднього
                            float currentY = startY + j * houseSpacing;
                            houseRect.setPosition(tile->getVisualPosition().x + xSideOffset, currentY);
                            window.draw(houseRect);
                        }
                    }
                }
            }
        }
    }
}

void Game::run() {
    if (!isRunning) {
        return;
    }

    sf::Clock clock; // Для розрахунку deltaTime (якщо потрібен для update)

    while (window.isOpen()) {
        sf::Time deltaTime = clock.restart();

        processEvents();
        update(deltaTime);
        render();
    }
}
