//
// Created by Legion on 19.06.2025.
//

#include "Game.h"
#include "BoardTile.h"
#include <iostream>
#include <vector>
#include <string>

Game::Game()
    : window(),
      mainFont(),
      boardTexture(),
      boardSprite(),
      gameBoard(),
      players(),
      randomEngine(std::random_device{}()),
      diceDistribution(1, 6),
      boardWidth(0.f),
      chatAreaX(0.f),
      chatAreaWidth(0.f),
      maxLogMessages(150),
      logCharacterSize(14),
      currentPlayerIndex(0),
      doublesRolledCount(0),
      chatScrollOffset(0.f),
      totalLogTextHeight(0.f),
      isRunning(false),
      rollDoneThisTurn(false)
{
    if (!loadResources()) {
        std::cout << "Failed to load resources!" << std::endl;
        return;
    }

    float const boardTextureHeight = static_cast<float>(boardTexture.getSize().y);
    this->boardWidth = static_cast<float>(boardTexture.getSize().x);
    this->chatAreaWidth = CHAT_WIDTH;
    this->chatAreaX = this->boardWidth + PADDING;

    unsigned int windowWidth = static_cast<unsigned int>(this->boardWidth + PADDING  + this->chatAreaWidth);
    unsigned int windowHeight = static_cast<unsigned int>(boardTextureHeight);

    window.create(sf::VideoMode(windowWidth, windowHeight), "Monopoly by Nazar");
    window.setVerticalSyncEnabled(true);

    boardSprite.setTexture(boardTexture);
    boardSprite.setPosition(0.f, 0.f);

    initializeUI();
    initializePlayers(2);
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

void Game::initializeUI() {
    float chatDisplayHeight = 300.f;
    if (chatDisplayHeight > static_cast<float>(window.getSize().y) - 2 * PADDING) {
        chatDisplayHeight = static_cast<float>(window.getSize().y) - 2 * PADDING;
    }
    if (chatDisplayHeight <= 0) chatDisplayHeight = 50.f;

    chatBackground.setSize(sf::Vector2f(this->chatAreaWidth - 2 * Game::PADDING, chatDisplayHeight));
    chatBackground.setPosition(this->chatAreaX + Game::PADDING, Game::PADDING);
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
void Game::addPlayerActionToLog(const Player& player, const std::string& actionMessagePart) {

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
void Game::addSystemMessageToLog(const std::string& message) {

    LogEntry  entry;
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

    float maxScrollOffset  = totalLogTextHeight - chatView.getSize().y;

    if (maxScrollOffset  < 0.f) maxScrollOffset  = 0.f;
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
        sf::Color playerColor = (i < playerColors.size() ) ? playerColors[i] : sf::Color::Black;  // Колір за замовчуванням

        players.emplace_back(playerName, 1500, playerColor);

        if (const BoardTile* goTile = gameBoard.getTileAt(gameBoard.getGoPosition())) {
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

    const BoardTile* tile = gameBoard.getTileAt(tileIndex);
    if (!tile) return;

    sf::Vector2f centerOfTile = tile->getVisualPosition();

    std::vector<Player*> playersOnThisTile;
    for (Player& p : players) {
        if (p.getCurrentPosition() == tileIndex && !p.getIsBankrupt()) {
            playersOnThisTile.push_back(&p);
        }
    }

    int numPlayersOnTile = playersOnThisTile.size();
    if (numPlayersOnTile == 0) {
        return;
    }

    const float baseOffset = 20.f;
    std::vector<sf::Vector2f> offsets;

    if (numPlayersOnTile == 1) {
        offsets.push_back({0.f, 0.f});
    } else if (numPlayersOnTile == 2) {
        offsets.push_back({-baseOffset, 0.f}); // Перший ліворуч
        offsets.push_back({baseOffset, 0.f});  // Другий праворуч
    } else if (numPlayersOnTile == 3) {
        offsets.push_back({0.f, -baseOffset});         // Вгорі
        offsets.push_back({-baseOffset, baseOffset / 2.f}); // Внизу зліва
        offsets.push_back({baseOffset, baseOffset / 2.f});  // Внизу справа
    } else if (numPlayersOnTile >= 4) {
        offsets.push_back({-baseOffset, -baseOffset}); // Верхній лівий
        offsets.push_back({baseOffset, -baseOffset});  // Верхній правий
        offsets.push_back({-baseOffset, baseOffset});  // Нижній лівий
        offsets.push_back({baseOffset, baseOffset});   // Нижній правий
    }

    for (size_t i = 0; i < playersOnThisTile.size(); ++i) {
        if (i < offsets.size()) {
            playersOnThisTile[i]->setTokenVisualPosition(centerOfTile + offsets[i]);
        } else {
            playersOnThisTile[i]->setTokenVisualPosition(centerOfTile + sf::Vector2f(static_cast<float>(i-3)*3.f, static_cast<float>(i-3)*3.f)); // Невеликий діагональний зсув для решти
        }
    }
}

void Game::processEvents() {
    sf::Event event {};
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
            isRunning = false;
        }

        if (event.type == sf::Event::MouseWheelScrolled) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            sf::FloatRect chatAreaRect = chatBackground.getGlobalBounds();

            if (chatAreaRect.contains(static_cast<sf::Vector2f>(mousePos))) {
                handleChatScroll(event.mouseWheelScroll.delta);
            }
        }

        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape) {
                window.close();
                isRunning = false;
            }

            if (event.key.code == sf::Keyboard::Space && !players.empty()) {
                Player& currentPlayerRef = players[currentPlayerIndex];

                if (this->rollDoneThisTurn) {
                    addPlayerActionToLog(currentPlayerRef, "has already rolled this turn.");
                    continue;
                }

                int die1 = diceDistribution(randomEngine);
                int die2 = diceDistribution(randomEngine);
                int diceRoll = die1 + die2;
                bool isDouble = (die1 == die2);

                std::string rollMsg = "rolled " + std::to_string(die1) + " + " + std::to_string(die2) +
                                      " = " + std::to_string(diceRoll) + (isDouble ? " (DOUBLES!)" : "");
                addPlayerActionToLog(currentPlayerRef, rollMsg);

                bool passedGo = false;
                int oldPosition = currentPlayerRef.getCurrentPosition();
                int newPosition = gameBoard.movePlayer(currentPlayerRef, diceRoll, passedGo);
                if (oldPosition != newPosition) { // Якщо гравець дійсно перемістився
                    updateTokenPositionsOnTile(oldPosition);
                }
                updateTokenPositionsOnTile(newPosition);

                if (passedGo) {
                    addPlayerActionToLog(currentPlayerRef, "collected $200 for passing GO!");
                    currentPlayerRef.addMoney(200);
                }

                std::vector<Player*> allPlayersPtrs;
                for (Player& p : players) { allPlayersPtrs.push_back(&p); }

                gameBoard.playerLandedOnSquare(currentPlayerRef, newPosition, allPlayersPtrs);
                rollDoneThisTurn = true;

                if (!isDouble) {
                    this->currentPlayerIndex = (this->currentPlayerIndex + 1) % static_cast<int>(players.size());
                    rollDoneThisTurn = false;
                    this->doublesRolledCount = 0;
                        if (!players.empty()) {
                            addSystemMessageToLog("--- Next turn ---");
                            addPlayerActionToLog(players[this->currentPlayerIndex], "it's your turn.");
                         }
                } else {
                    this->doublesRolledCount++;
                    addPlayerActionToLog(currentPlayerRef, "rolled doubles! Times: " + std::to_string(this->doublesRolledCount));
                    if (this->doublesRolledCount == 3) {
                        addPlayerActionToLog(currentPlayerRef, "rolled 3 doubles and GOES TO JAIL!");
                        currentPlayerRef.sendToJail(gameBoard.getJailPosition());

                        if(const BoardTile* jailTile = gameBoard.getTileAt(gameBoard.getJailPosition())) {
                            currentPlayerRef.setTokenVisualPosition(jailTile->getVisualPosition());
                        }

                        this->currentPlayerIndex = (this->currentPlayerIndex + 1) % static_cast<int>(players.size());
                        this->rollDoneThisTurn = false;
                        this->doublesRolledCount = 0;

                        if (!players.empty()) {
                            addSystemMessageToLog("--- Next turn ---");
                            addPlayerActionToLog(players[this->currentPlayerIndex], "it's your turn.");
                        }
                    } else {
                        this->rollDoneThisTurn = false;
                        addPlayerActionToLog(currentPlayerRef, "gets another turn.");
                    }
                }
            }
        }
    }
}

void Game::update(sf::Time deltaTime) {
    // тіпа під анімації, пока ніц не надо
}

void Game::render() {
    window.clear(sf::Color(50,50,50));
    window.draw(boardSprite);

    for (const auto& player : players) {
        player.drawToken(window);
    }

    window.draw(chatBackground);

    sf::View currentMainView  = window.getView();
    sf::Vector2f viewSize = chatView.getSize();
    chatView.setCenter(viewSize.x / 2.f, viewSize.y / 2.f + chatScrollOffset);
    window.setView(this->chatView);

    const float lineHeight = static_cast<float>(logCharacterSize) + 6.f;
    const float paddingX_in_view = 5.f;
    float currentTextLine_Y_in_view = totalLogTextHeight - paddingX_in_view;

    for (auto entryIt = logEntries.rbegin(); entryIt != logEntries.rend(); ++entryIt) {
        LogEntry& entry = *entryIt;
        float currentSegmentX_in_view = paddingX_in_view;

        sf::FloatRect part1Bounds = entry.part1.getLocalBounds();
        entry.part1.setPosition(currentSegmentX_in_view, currentTextLine_Y_in_view - part1Bounds.height - part1Bounds.top);
        window.draw(entry.part1);

        if (entry.isTwoPartMessage) {
            currentSegmentX_in_view += entry.part1.getGlobalBounds().width;
            sf::FloatRect part2Bounds = entry.part2.getLocalBounds();
            entry.part2.setPosition(currentSegmentX_in_view, currentTextLine_Y_in_view - part2Bounds.height - part2Bounds.top);
            window.draw(entry.part2);
        }
        currentTextLine_Y_in_view -= lineHeight;
    }
    window.setView(currentMainView);
    window.display();
}

void Game::run() {
    if (!isRunning) {
        return;
    }

    sf::Clock clock;  // Для розрахунку deltaTime (якщо потрібен для update)

    while (window.isOpen()) {
        sf::Time deltaTime = clock.restart();

        processEvents();
        update(deltaTime);
        render();
    }
}


