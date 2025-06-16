//
// Created by Legion on 28.05.2025.
//

#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>
#include <string>

class Button {
private:
    sf::RectangleShape button;  // Об'єкт, що представляє видимий прямокутник кнопки
    sf::Text text;             // Об'єкт для відображення тексту на кнопці

public:
    Button(const std::string& t, sf::Vector2f size, int charSize, const sf::Font& fontRef, sf::Color bgColor, sf::Color textColor);

    // Метод для отримання поточного кольору фону кнопки
    sf::Color getBackColor() const;

    // Метод для встановлення шрифту тексту на кнопці
    void setFont(const sf::Font& fontRef);

    // Метод для встановлення кольору фону кнопки
    void setBackColor(sf::Color color);

    // Метод для встановлення кольору тексту на кнопці
    void setTextColor(sf::Color color);

    // Метод для встановлення позиції кнопки та центрування тексту
    void setPosition(sf::Vector2f pos);

    // Метод для малювання кнопки на переданому вікні sf::RenderWindow
    void drawTo(sf::RenderWindow &window) const;

    // Метод для перевірки, чи знаходиться курсор миші над кнопкою
    bool isMouseOver(const sf::RenderWindow &window) const;
};

#endif //BUTTON_H
