//
// Created by Legion on 19.06.2025.
//

#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <string>
#include <memory>
#include <set>

#include "Player.h"
#include "GameBoard.h"
#include "ButtonManager.h"


class Game {
public:
    // UI
    static constexpr float UI_AREA_WIDTH = 300.f;
    static constexpr float CHAT_AREA_HEIGHT  = 300.f;
    static constexpr float PLAYER_STATUS_AREA_HEIGHT = 150.f;
    static constexpr float PADDING = 10.f;

    enum class GameState {
        NORMAL_TURN,
        AWAITING_PROPERTY_SELECTION_FOR_MORTGAGE,
        AWAITING_PROPERTY_SELECTION_FOR_UNMORTGAGE,
        AWAITING_PROPERTY_SELECTION_FOR_BUILDING,
        AWAITING_PROPERTY_SELECTION_FOR_SELL_HOUSE,
        CONFIRMATION_DIALOG
    };

private:
    GameState currentGameState;
    GameState stateBeforeConfirmation;
    sf::RectangleShape selectionHighlight;
    sf::RectangleShape confirmationDialogBackground;
    sf::Text confirmationText;
    std::unique_ptr<Button> confirmButton;
    std::unique_ptr<Button> cancelButton;
    Street* selectedStreet;

    sf::RectangleShape propertyOwnerHighlight;  // прямокутник для підсвічування куплених вулиць гравців

    sf::RenderWindow window;
    sf::Font mainFont;
    sf::Texture boardTexture;
    sf::Sprite boardSprite;
    GameBoard gameBoard;
    std::vector<Player> players;

    // generate random numbers
    std::mt19937 randomEngine;
    std::uniform_int_distribution<int> diceDistribution;

    // UI
    std::unique_ptr<UIManager> uiManager;

    // Player panel
    sf::RectangleShape playerStatusBackground;
    struct PlayerStatusUI {
        sf::CircleShape turnIndicator;
        sf::Text nameText;
        sf::Text moneyText;
        sf::Text jailCardText;
    };
    std::vector<PlayerStatusUI> playerStatusUIs;

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
    unsigned int maxLogMessages;  // максимальна к-ть повідомлень у логу
    unsigned int logCharacterSize;  // розмір символів тексту
    float chatScrollOffset;
    float totalLogTextHeight;

    float boardActualWidth;
    float uiPanelX;

    std::set<Street::StreetColorGroup> groupsBuiltOnThisTurn;

    int currentPlayerIndex;  // індекс поточного гравця у векторі
    int doublesRolledCount;  // лічильник кинутих дублів
    bool isRunning;  // чи запущена гра
    bool rollDoneThisTurn;  // чи зробив гравець кидок кубоків в ході
    bool playerJustLanded; // перевірка на позицію гравця на новий раунд

    [[nodiscard]] sf::FloatRect getTileBounds(int tileIndex) const;


    bool loadResources();  // для заантаження шрифтів
    void initializePlayers(int numberOfPlayers);  // для створення гравців
    void initializeGameUIAndButtons();  // UI
    void initializePlayerStatusUI(); // Player Panel
    void updateTokenPositionsOnTile(int tileIndex);
    void processEvents();  // оборка подій
    void update(sf::Time deltaTime);  // оновлення логіки гри
    void render();  // малювання всього на екрані
    // методи додавання повідомлень в лог
    void updateScrollAfterNewMessage();
    void addPlayerActionToLog(const Player& player, const std::string& actionMessagePart);
    void addSystemMessageToLog(const std::string& message);
    void handleChatScroll(float delta);
    void handleButtonAction(GameButtonType actionType); // обробка дій кнопок
    // Button
    void enterMortgageSelectionMode();
    void enterUnmortgageSelectionMode();
    void enterBuildSelectionMode();
    void enterSellHouseSelectionMode();
    void handlePropertySelectionForMortgage(sf::Vector2i mousePosition);
    void handlePropertySelectionForUnmortgage(sf::Vector2i mousePosition);
    void handlePropertySelectionForBuilding(sf::Vector2i mousePosition);
    void handlePropertySelectionForSellHouse(sf::Vector2i mousePosition);

    void showConfirmationDialog(const std::string& message, GameButtonType confirmAction, GameButtonType cancelAction);
    void hideConfirmationDialog();
    void renderSelectionHighlights();
    void renderConfirmationDialog();
    void renderOwnedProperties(); // метод для підсвучання куплених вулиць

public:
    Game();
    ~Game() = default;

    void run();
};
#endif //GAME_H
