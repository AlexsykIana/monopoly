#include "Button.h"

Button::Button(const std::string &t, sf::Vector2f size, int charSize, const sf::Font &fontRef, sf::Color bgColor,
               sf::Color textColor) {
    text.setString(t);
    text.setFont(fontRef);
    text.setFillColor(textColor);
    text.setCharacterSize(charSize);

    button.setSize(size);
    button.setFillColor(bgColor);
    isActive = true;
}

sf::FloatRect Button::getGlobalBounds() const {
    return button.getGlobalBounds();
}

sf::Color Button::getBackColor() const {
    return button.getFillColor();
}

bool Button::getIsActive() const {
    return isActive;
}

void Button::setActive(bool active) {
    isActive = active;

    if (!isActive) {
        setBackColor(sf::Color(150, 150, 150));
        setTextColor(sf::Color(0, 0, 0));
    }
}

void Button::setFont(const sf::Font &fontRef) {
    text.setFont(fontRef);
}

void Button::setBackColor(sf::Color color) {
    button.setFillColor(color);
}

void Button::setTextColor(sf::Color color) {
    text.setFillColor(color);
}

void Button::setPosition(sf::Vector2f pos) {
    button.setPosition(pos);

    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    text.setPosition(pos.x + button.getSize().x / 2.0f, pos.y + button.getSize().y / 2.0f);
}

void Button::drawTo(sf::RenderWindow &window) const {
    window.draw(button);
    window.draw(text);
}

bool Button::isMouseOver(const sf::RenderWindow &window) const {
    return button.getGlobalBounds().contains(
        static_cast<float>(sf::Mouse::getPosition(window).x),
        static_cast<float>(sf::Mouse::getPosition(window).y)
    );
}
