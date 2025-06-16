#include <SFML/Graphics.hpp>
#include <iostream>

#include "Button.h"
#include "Player.h"
#include "Street.h"


int main()
{
    // Завантаження текстури для фону
    sf::Texture boardTexture;
    // вказую шлях до мого фону(якщо не знайде фотографію то виведе помилку)
    if (!boardTexture.loadFromFile("C:/projects/monopoly/assets/monopoly-2.jpg")) {
        std::cout << "Failed to load monopoly" << std::endl;
        return -1;
    }

    // Завантаження шрифта
    sf::Font myFont;
    // вказую шлях до мого шрифта
    if (!myFont.loadFromFile("C:/projects/monopoly/assets/fonts/Roboto_Condensed-Regular.ttf")) {
        std::cout << "Error loading font!" << std::endl;
        return -1;
    }

    // Створення спрайту для фону
    sf::Sprite boardSprite(boardTexture);
    // автоматично бере розмір текстури і буде розташований в (0,0) по координатам

    // Створюємо вікно
    sf::RenderWindow window(sf::VideoMode(boardTexture.getSize().x, boardTexture.getSize().y), "Monopoly Nazar");
    window.setVerticalSyncEnabled(true); // Додає плавності

    // Створюємо просту фігуру для відображення
    sf::CircleShape shape(20.f);  // розмір колі
    shape.setFillColor(sf::Color::Green);  // колір фігури
    shape.setPosition( 650.f, 35.f); // Центруємо фігуру на кллітинку *GO*

    // Кліпаєм кнопку
    Button btn1("ROLL", { 200.f, 75.f}, 50, myFont,  sf::Color(50, 0, 200, 255), sf::Color::Black);
    Button btn2("BUY", { 200.f, 75.f}, 50, myFont,  sf::Color(0, 200, 0, 255), sf::Color::Black);
    Button btn3("SELL", {200.f, 75.f}, 50, myFont, sf::Color(200, 0, 0, 255), sf::Color::Black);
    // вказую координати кнопки
    btn1.setPosition(sf::Vector2f(200.f, 250.f));
    btn2.setPosition(sf::Vector2f(200.f, 450.f));
    btn3.setPosition(sf::Vector2f(500.f, 450.f));

    // Запам'ятовуємо початкові (idle) кольори для кожної кнопки
    sf::Color btn1IdleColor = btn1.getBackColor();
    sf::Color btn2IdleColor = btn2.getBackColor();
    sf::Color btn3IdleColor = btn3.getBackColor();

    // Кольори для кнопки
    sf::Color buttonHoverColor = sf::Color(220, 220, 180, 255);
    sf::Color buttonDisabledColor = sf::Color(128, 128, 128, 255);

    // Стан гри
    bool rollDoneThisTurn = false;


    Button* hoveredButton = nullptr;  // Яка кнопка зараз під курсором (для hover ефекту)
    Button* buttonPressedDownOn = nullptr; // Яка кнопка була під курсором, КОЛИ мишу натиснули

    // Головний цикл програми, виконується, доки вікно відкрите
    while (window.isOpen()) {

        // --- ОБРОБКА ПОДІЙ ---
        // На кожній ітерації головного циклу ми перевіряємо, чи сталися якісь події
        // (натискання клавіш, рух миші, закриття вікна тощо).
        sf::Event event{};  // Створення об'єкта події та його ініціалізація за замовчуванням

        // Внутрішній цикл для обробки всіх подій, які накопичилися в черзі з моменту останньої перевірки.
        // window.pollEvent(event) витягує одну подію з черги (якщо вона є),
        // записує її в 'event' і повертає true. Якщо черга порожня, повертає false.
        while (window.pollEvent(event)) { // pollEvent витягує події з черги

            // Аналіз типу події за допомогою оператора switch
            switch (event.type) {
                case sf::Event::Closed:  // Випадок: користувач натиснув кнопку "закрити" на вікні
                    window.close();
                    break;

                case sf::Event::KeyPressed:  // Перевірка, чи була натиснута клавіша Escape
                    if (event.key.code == sf::Keyboard::Escape) {
                        window.close();
                    }
                    break;

///////////////////////////////Main Tasks/////////////////////////////////////////////////////////

                case sf::Event::MouseMoved:
                    if (!buttonPressedDownOn) {  // перевірка чи натиснута ліва кнопка при наведені
                        // Спочатку скидаємо hover ефект з попередньої кнопки (якщо вона не btn1 і rollDoneThisTurn)
                        if (hoveredButton) {  // скидаєм до початвих кольорів
                            // Якщо hoveredButton була btn1 і rollDoneThisTurn, її колір вже сірий, не чіпаємо
                            if (hoveredButton == &btn1 && !rollDoneThisTurn) hoveredButton->setBackColor(btn1IdleColor);
                            else if (hoveredButton == &btn1 && rollDoneThisTurn) hoveredButton->setBackColor(buttonDisabledColor);
                            else if (hoveredButton == &btn2) hoveredButton->setBackColor(btn2IdleColor);
                            else if (hoveredButton == &btn3) hoveredButton->setBackColor(btn3IdleColor);
                            hoveredButton = nullptr;  // Більше немає активної кнопки від MouseMoved
                        }

                        // перевірка при наведенні на кожну кнопку
                        if (btn1.isMouseOver(window) && !rollDoneThisTurn) {
                            btn1.setBackColor(buttonHoverColor);
                            hoveredButton = &btn1;
                        } else if (btn2.isMouseOver(window)) {
                            btn2.setBackColor(buttonHoverColor);
                            hoveredButton = &btn2;
                        } else if (btn3.isMouseOver(window)) {
                            btn3.setBackColor(buttonHoverColor);
                            hoveredButton = &btn3;
                        }
                    }
                    break;

                    // Натиснута кнопка миші
                case sf::Event::MouseButtonPressed: // ЗАЛИШАЄМО ДЛЯ ВІДСТЕЖЕННЯ, АЛЕ БЕЗ ЗМІНИ КОЛЬОРУ/ДІЇ
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        buttonPressedDownOn = nullptr; // Скидаємо
                        if (btn1.isMouseOver(window) && !rollDoneThisTurn) {
                            buttonPressedDownOn = &btn1;
                        } else if (btn2.isMouseOver(window)) {
                            buttonPressedDownOn = &btn2;
                        } else if (btn3.isMouseOver(window)) {
                            buttonPressedDownOn = &btn3;
                        }
                    }
                    break;

                 case sf::Event::MouseButtonReleased:
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (buttonPressedDownOn) { // Якщо кнопка була натиснута
                            // Перевіряємо, чи курсор ВСЕ ЩЕ над тією ж кнопкою, над якою відбулося натискання
                            if (buttonPressedDownOn->isMouseOver(window)) {
                                // Це вважається кліком
                                if (buttonPressedDownOn == &btn1) { // Вже перевірено !rollDoneThisTurn у MouseButtonPressed
                                    buttonPressedDownOn->setBackColor(buttonDisabledColor);
                                    rollDoneThisTurn = true;
                                    std::cout << "Button 'ROLL' action on release!" << std::endl;
                                } else if (buttonPressedDownOn == &btn2) {
                                    // Дія для btn2
                                    std::cout << "Button 'BUY' action on release!" << std::endl;
                                    buttonPressedDownOn->setBackColor(buttonHoverColor); // Залишити hover, якщо курсор над нею
                                } else if (buttonPressedDownOn == &btn3) {
                                    // Дія для btn3
                                    std::cout << "Button 'SELL' action on release!" << std::endl;
                                    buttonPressedDownOn->setBackColor(buttonHoverColor); // Залишити hover
                                }
                            } else {
                                // Кнопку натиснули над buttonPressedDownOn, але відпустили поза нею
                                // Повертаємо idle колір для buttonPressedDownOn (якщо це не btn1, яка вже disabled)
                                if (buttonPressedDownOn == &btn1 && !rollDoneThisTurn) buttonPressedDownOn->setBackColor(btn1IdleColor);
                                else if (buttonPressedDownOn == &btn1 && rollDoneThisTurn) buttonPressedDownOn->setBackColor(buttonDisabledColor); // Залишається сірою
                                else if (buttonPressedDownOn == &btn2) buttonPressedDownOn->setBackColor(btn2IdleColor);
                                else if (buttonPressedDownOn == &btn3) buttonPressedDownOn->setBackColor(btn3IdleColor);
                            }
                            // Оновлюємо hoveredButton після всіх дій
                            // ( MouseMoved event після Release подбає про це, якщо курсор рухався,
                            // але для надійності можна перевірити тут)
                            if (buttonPressedDownOn->isMouseOver(window)) {
                                hoveredButton = buttonPressedDownOn;
                            } else {
                                // Якщо курсор пішов з кнопки, на яку натискали, то перевіряємо інші
                                if (btn1.isMouseOver(window) && !rollDoneThisTurn) hoveredButton = &btn1;
                                else if (btn2.isMouseOver(window)) hoveredButton = &btn2;
                                else if (btn3.isMouseOver(window)) hoveredButton = &btn3;
                                else hoveredButton = nullptr;
                            }
                        }
                        buttonPressedDownOn = nullptr; // Завжди скидаємо після відпускання
                    }
                    break;

                default:
                    // Інші типи подій, які нас зараз не цікавлять
                    break;
            }
        }
        // Очищення вікна (заливаємо чорним кольором)
        window.clear(sf::Color::Black);

        // Малюється все по черзі(спочатку фон а потім фігури, так як фон їх перекриває)

        window.draw(boardSprite); // малюєм фон
        btn1.drawTo(window);
        btn2.drawTo(window);
        btn3.drawTo(window);
        window.draw(shape);  // Малюємо нашу фігуру



        // Відображаємо намальоване на екрані
        window.display();
    }

    return 0;
}