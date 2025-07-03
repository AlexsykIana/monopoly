#include "ButtonManager.h"
#include "Game.h"
#include "Player.h"
#include <iostream>


UIManager::UIManager(sf::Font &font, float ui_panel_x, float buttons_start_y)
    : uiFont(font),
      hoverColor(sf::Color(210,210,210,255)),
      disabledColor(sf::Color(120,120,120,200)),
      panelX(ui_panel_x),
      panelY(buttons_start_y),
      buttonSize(100.f,40.f),
      buttonCharSize(12),
      buttonSpacing(10.f)
{
    initializeAllButtons();
}

void UIManager::createButton(GameButtonType  type, const std::string &text, sf::Color bgColor, sf::Color textColor, sf::Vector2f position) {
    auto result = buttons.emplace(std::piecewise_construct,
                               std::forward_as_tuple(type),
                               std::forward_as_tuple(text, buttonSize, buttonCharSize, uiFont, bgColor, textColor));

    result.first->second.setPosition(position);
    idleButtonColors[type] = bgColor;
}

void UIManager::initializeAllButtons() {
    float currentY_col1 = panelY;
    float currentY_col2 = panelY;

    const float x_col1 = panelX + Game::PADDING;
    const float x_col2 = x_col1 + buttonSize.x + buttonSpacing;

    // Лівий стовбець
    createButton(GameButtonType ::ROLL, "ROLL DICE",
        sf::Color(70,70,200), sf::Color::White, {x_col1, currentY_col1});
    currentY_col1 += buttonSize.y + buttonSpacing;

    createButton(GameButtonType ::BUY, "BUY",
        sf::Color(0,150,0), sf::Color::White, {x_col1, currentY_col1});
    currentY_col1 += buttonSize.y + buttonSpacing;

    createButton(GameButtonType ::PAY_OR_USE_CARD_JAIL, "PAY/USE CARD JAIL",
        sf::Color(218,165,32), sf::Color::White, {x_col1, currentY_col1});
    currentY_col1 += buttonSize.y + buttonSpacing;

    createButton(GameButtonType ::END_TURN, "END TURN",
        sf::Color(150,0,0), sf::Color::White, {x_col1, currentY_col1});

    // Правий стовбець
    createButton(GameButtonType ::MORTGAGE, "MORTGAGE",
                 sf::Color(200,100,0), sf::Color::White,{x_col2, currentY_col2});
    currentY_col2 += buttonSize.y + buttonSpacing;

    createButton(GameButtonType ::UNMORTGAGE, "UNMORTGAGE",
                sf::Color(220,150,50), sf::Color::White,{x_col2, currentY_col2});
    currentY_col2 += buttonSize.y + buttonSpacing;

    createButton(GameButtonType ::SELL_HOUSE, "SELL_HOUSE",
                sf::Color(180,0,0), sf::Color::White,{x_col2, currentY_col2});
    currentY_col2 += buttonSize.y + buttonSpacing;

    createButton(GameButtonType ::BUILD_HOUSE, "BUILD HOUSE",
                sf::Color(0,150,200), sf::Color::White,{x_col2, currentY_col2});




    for (auto& pair : buttons) {
        pair.second.setActive(false);
    }

    if (buttons.count(GameButtonType ::ROLL)) buttons.at(GameButtonType ::ROLL).setActive(true);

}

void UIManager::updateButtonsStates(const Player &currentPlayer, bool canRoll, bool canBuy, bool canMortgage, bool canUnmortgage, bool canBuild,
    bool canSellHouse, bool canPayOrUseCardForJail, bool canManageProperties, bool canEndTurn, bool isInJail) {

    if (buttons.count(GameButtonType ::ROLL)) {
        buttons.at(GameButtonType ::ROLL).setActive(canRoll);
    }

    if (buttons.count(GameButtonType ::BUY)) {
        buttons.at(GameButtonType ::BUY).setActive(canBuy);
    }

    if (buttons.count(GameButtonType ::END_TURN)) {
        buttons.at(GameButtonType ::END_TURN).setActive(canEndTurn);
    }

    if (buttons.count(GameButtonType ::SELL_HOUSE)) {
        buttons.at(GameButtonType ::SELL_HOUSE).setActive(canSellHouse);
    }

    if (buttons.count(GameButtonType ::MORTGAGE)) {
        buttons.at(GameButtonType ::MORTGAGE).setActive(canMortgage);
    }

    if (buttons.count(GameButtonType ::UNMORTGAGE)) {
        buttons.at(GameButtonType ::UNMORTGAGE).setActive(canUnmortgage);
    }

    if (buttons.count(GameButtonType ::BUILD_HOUSE)) {
        buttons.at(GameButtonType ::BUILD_HOUSE).setActive(canBuild);
    }

    if (buttons.count(GameButtonType ::PAY_OR_USE_CARD_JAIL)) {
        buttons.at(GameButtonType ::PAY_OR_USE_CARD_JAIL).setActive(canPayOrUseCardForJail);
    }
}

GameButtonType  UIManager::handleMouseClick(sf::Vector2i mousePosition, const sf::RenderWindow &window) const {
    for (const auto&[fst, snd] : buttons) {
        if (snd.getIsActive() && snd.isMouseOver(window)) {
            std::cout << "Button clicked: " << static_cast<int>(fst) << std::endl;
            return fst;
        }
    }
    return GameButtonType ::NONE;
}

void UIManager::handleMouseHover(sf::Vector2i mousePosition, const sf::RenderWindow &window) {
    for (auto& pair : buttons) {
        if (!pair.second.getIsActive()) {
            pair.second.setBackColor(disabledColor);
        } else {
            if (pair.second.isMouseOver(window)) {
                pair.second.setBackColor(hoverColor);
            } else {
                if (idleButtonColors.count(pair.first)) {
                    pair.second.setBackColor(idleButtonColors.at(pair.first));
                } else {
                    pair.second.setBackColor(sf::Color::Cyan);
                }
            }
        }
    }
}

void UIManager::draw(sf::RenderWindow &window) const {
    for (const auto& pair : buttons) {
        pair.second.drawTo(window);
    }
}
