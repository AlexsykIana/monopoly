//
// Created by Legion on 25.06.2025.
//

#ifndef BUTTONMANAGER_H
#define BUTTONMANAGER_H

#include "Button.h"
#include <SFML/Graphics.hpp>
#include <map>
#include <string>

enum class GameButtonType  {
    NONE = -1,
    ROLL,
    BUY,
    SELL_HOUSE,
    MORTGAGE,
    UNMORTGAGE,
    BUILD_HOUSE,
    END_TURN,
    PAY_OR_USE_CARD_JAIL
};

class Player;

class UIManager {
private:
    sf::Font& uiFont;

    std::map<GameButtonType , Button> buttons;
    std::map<GameButtonType , sf::Color> idleButtonColors;

    sf::Color hoverColor;
    sf::Color disabledColor;

    float panelX;
    float panelY;
    sf::Vector2f buttonSize;
    int buttonCharSize;
    float buttonSpacing;

    void createButton(GameButtonType  type, const std::string& text, sf::Color bgColor, sf::Color textColor, sf::Vector2f position);

public:
    UIManager(sf::Font& font, float ui_panel_x, float buttons_start_y);

    void initializeAllButtons();

    void updateButtonsStates(const Player& currentPlayer, bool canRoll, bool canBuy, bool canMortgage, bool canUnmortgage, bool canBuild,
                             bool canSellHouse, bool canPayOrUseCardForJail, bool canManageProperties, bool canEndTurn, bool isInJail);

    [[nodiscard]] GameButtonType  handleMouseClick(sf::Vector2i mousePosition, const sf::RenderWindow& window) const;

    void handleMouseHover(sf::Vector2i mousePosition, const sf::RenderWindow& window);

    void draw(sf::RenderWindow& window) const;

};
#endif //BUTTONMANAGER_H
