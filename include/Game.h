//
// Created by Legion on 19.06.2025.
//

#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <string>

#include "Player.h"
#include "GameBoard.h"

class Game {
public:
    static constexpr float UI_AREA_WIDTH = 300.f;
    static constexpr float CHAT_WIDTH = 300.f;
    static constexpr float PADDING = 10.f;

private:
    sf::RenderWindow window;
    sf::Font mainFont;
    sf::Texture boardTexture;
    sf::Sprite boardSprite;
    GameBoard gameBoard;
    std::vector<Player> players;

    // generate random numbers
    std::mt19937 randomEngine;
    std::uniform_int_distribution<int> diceDistribution;

    //game log
    sf::RectangleShape chatBackground;
    struct LogEntry {
        sf::Text part1;
        sf::Text part2;
        bool isTwoPartMessage;

        LogEntry() : isTwoPartMessage(false) {}
    };
    std::vector<LogEntry> logEntries;  // для зберігання записів  логу

    // для скролу чату
    sf::View chatView;
    float chatScrollOffset;
    float totalLogTextHeight;

    float boardWidth;
    float chatAreaX;
    float chatAreaWidth;
    unsigned int maxLogMessages;  // максимальна к-ть повідомлень у логу
    unsigned int logCharacterSize;  // розмір символів тексту

    int currentPlayerIndex;  // індекс поточного гравця у векторі
    int doublesRolledCount;  // лічильник кинутих дублів
    bool isRunning;  // чи запущена гра
    bool rollDoneThisTurn;  // чи зробив гравець кидок кубоків в ході

    bool loadResources();  // для заантаження шрифтів
    void initializePlayers(int numberOfPlayers);  // для створення гравців
    void initializeUI();  // UI включає чат
    void updateTokenPositionsOnTile(int tileIndex);
    void processEvents();  // оборка подій
    void update(sf::Time deltaTime);  // оновлення логіки гри
    void render();  // малювання всього на екрані
    // методи додавання повідомлень в лог
    void updateScrollAfterNewMessage();
    void addPlayerActionToLog(const Player& player, const std::string& actionMessagePart);
    void addSystemMessageToLog(const std::string& message);
    void handleChatScroll(float delta);

public:
    Game();
    ~Game() = default;

    void run();
};
#endif //GAME_H
